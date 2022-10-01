#include "compresswindow.h"
#include "ui_compresswindow.h"

#include <QFileDialog>

CompressWindow::CompressWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::CompressWindow) {
  ui->setupUi(this);
  setModal(true);

  QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());

  connect(ui->saveAsButton, &QPushButton::clicked, this, &CompressWindow::saveAs);

  ui->addFilesButton->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  ui->addFilesButton->setIconSize(iconSize);
  connect(ui->addFilesButton, &QPushButton::clicked, this, &CompressWindow::addFiles);

  ui->clearListButton->setIcon(
      style()->standardIcon(QStyle::SP_DialogDiscardButton));
  ui->clearListButton->setIconSize(iconSize);
  connect(ui->clearListButton, &QPushButton::clicked, ui->listWidget, &QListWidget::clear);

  ui->embedButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
  ui->embedButton->setIconSize(iconSize);
  connect(ui->embedButton, &QPushButton::clicked, this, &CompressWindow::compress);

  huffmanEncoding = new HuffmanEncoding();
  connect(huffmanEncoding, &HuffmanEncoding::started, this,
          [this] { ui->progressBar->setHidden(false); });

  connect(huffmanEncoding, &HuffmanEncoding::finished, this,
          [this] { ui->progressBar->setHidden(true); });

  connect(huffmanEncoding, &HuffmanEncoding::progressChanged, ui->textEdit, &QTextEdit::append);
  connect(huffmanEncoding, &HuffmanEncoding::progressCounted, ui->progressBar, &QProgressBar::setValue);
  connect(huffmanEncoding, &HuffmanEncoding::setEnabled, ui->clearListButton, &QPushButton::setEnabled);
  connect(huffmanEncoding, &HuffmanEncoding::setEnabled, ui->embedButton, &QPushButton::setEnabled);
  connect(huffmanEncoding, &HuffmanEncoding::setEnabled, ui->addFilesButton,&QPushButton::setEnabled);
  connect(huffmanEncoding, &HuffmanEncoding::setEnabled, ui->listWidget,&QPushButton::setEnabled);
  connect(huffmanEncoding, &HuffmanEncoding::setEnabled, ui->saveLocation,&QPushButton::setEnabled);
}
void CompressWindow::showEvent(QShowEvent *) {
  if (!huffmanEncoding->isRunning()) {
    ui->progressBar->setHidden(true);
    ui->progressBar->setValue(0);
    ui->textEdit->clear();
    ui->listWidget->clear();
    ui->saveLocation->clear();
  }
}
CompressWindow::~CompressWindow() {
  delete ui;
  delete huffmanEncoding;
}

void CompressWindow::disabledButton() {
  ui->clearListButton->setDisabled(true);
  ui->embedButton->setDisabled(true);
  ui->addFilesButton->setDisabled(true);
  ui->listWidget->setDisabled(true);
  ui->saveLocation->setDisabled(true);
}

void CompressWindow::addFiles() {
  ui->progressBar->setValue(0);
  // Select Files
  QStringList files = QFileDialog::getOpenFileNames(
      this, tr("Files to Compress"), QDir::homePath(), tr("All Files (*.*)"));

  if (files.isEmpty())
    return;

  QStringList::Iterator it = files.begin();
  // Proccessing List
  while (it != files.end()) {
    ui->listWidget->addItem(*it);
    ++it;
  }
}

void CompressWindow::saveAs() {
  QString outputfile = QFileDialog::getSaveFileName(
      this, tr("Save As"), QString(), tr("Frog File (*.frog)"));
  if (outputfile.isNull() || outputfile.isEmpty()) // Cancel
  {
    ui->textEdit->setText(tr("Operation Canceled.")); // Cancel operation
  } else                                          // get filename to save
  {
    if (!outputfile.endsWith(".frog")) {
      outputfile.append(".frog");
    }
    ui->saveLocation->setText(outputfile);
  }
}

void CompressWindow::compress() {
  if (ui->listWidget->count() > 0 && ui->saveLocation->text().length() > 0) {
    // Disabled Button
    disabledButton();

    // Init Compressor
    int64_t totalsize;
    totalsize = 0;

    ui->textEdit->setText(tr("Preparing File : "));

    QStringList list;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
      QListWidgetItem *item = ui->listWidget->item(i);
      totalsize = totalsize + QFileInfo(item->text()).size();
      list.append(item->text());
      ui->textEdit->append(item->text());
      // Do stuff!
    }

    // Starting
    ui->textEdit->append(tr("Compressing..."));
    huffmanEncoding->setInputFile(list);
    huffmanEncoding->setOutputFile(ui->saveLocation->text());
    huffmanEncoding->setTotalSize(totalsize);
    huffmanEncoding->start();
  } else {
    ui->textEdit->setText(tr("Please fill all requirement"));
  }
}
