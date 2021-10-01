#include "extractwindow.h"
#include "ui_extractwindow.h"

#include <QFileDialog>

ExtractWindow::ExtractWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::ExtractWindow) {
  ui->setupUi(this);
  setModal(true);

  connect(ui->extractButton, &QPushButton::clicked, this, &ExtractWindow::Extract);
  connect(ui->inputButton, &QPushButton::clicked, this, &ExtractWindow::pickInputFile);
  connect(ui->outputDirButton, &QPushButton::clicked, this, &ExtractWindow::pickOutputPath);

  huffmanDecoding = new HuffmanDecoding();
  connect(huffmanDecoding, &HuffmanDecoding::started, this,
          [this] { ui->progressBar->setHidden(false); });

  connect(huffmanDecoding, &HuffmanDecoding::finished, this,
          [this] { ui->progressBar->setHidden(true); });

  connect(huffmanDecoding, &HuffmanDecoding::progressChanged, ui->textEdit, &QTextEdit::append);
  connect(huffmanDecoding, &HuffmanDecoding::progressCounted, ui->progressBar, &QProgressBar::setValue);
  connect(huffmanDecoding, &HuffmanDecoding::setEnabled, ui->extractButton, &QPushButton::setEnabled);
  connect(huffmanDecoding, &HuffmanDecoding::setEnabled, ui->inputButton, &QPushButton::setEnabled);
  connect(huffmanDecoding, &HuffmanDecoding::setEnabled, ui->outputDirButton, &QPushButton::setEnabled);
}

void ExtractWindow::showEvent(QShowEvent *) {
  if (!huffmanDecoding->isRunning()) {
    ui->progressBar->setHidden(true);
    ui->progressBar->setValue(0);
    ui->inputFile->clear();
    ui->extractPosition->clear();
    ui->textEdit->clear();
  }
}

ExtractWindow::~ExtractWindow() {
  delete ui;
  delete huffmanDecoding;
}

void ExtractWindow::Extract() {
  ui->progressBar->setValue(0);
  QString filename = ui->inputFile->text();
  QString output = ui->extractPosition->text();
  if (!filename.isEmpty() && !output.isEmpty()) {
    ui->textEdit->setText("");
    huffmanDecoding->setInputFile(filename);
    huffmanDecoding->setOutputFile(output);
    huffmanDecoding->start();
  }
}

void ExtractWindow::pickInputFile() {
  QString filename =
      QFileDialog::getOpenFileName(this, tr("Open File to Extract..."),
                                   QDir::homePath(), "Frog File (*.frog)");
  if (!filename.isEmpty())
    ui->inputFile->setText(filename);
}

void ExtractWindow::pickOutputPath() {
  QString outputdir = QFileDialog::getExistingDirectory(
      this, tr("Choose Directory to Extract"), QDir::homePath());
  if (!outputdir.isEmpty())
    ui->extractPosition->setText(outputdir);
}
