#include "compresswindow.h"
#include "ui_compresswindow.h"

#include <QFileDialog>

CompressWindow::CompressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompressWindow)
{
    ui->setupUi(this);
    setModal(true);

    QSize iconSize = QSize (fontMetrics().height(), fontMetrics().height());
    ui->addFilesButton->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->addFilesButton->setIconSize(iconSize);
    ui->clearListButton->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    ui->clearListButton->setIconSize(iconSize);
    ui->embedButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->embedButton->setIconSize(iconSize);

    huffmanEncoding = new HuffmanEncoding();
    connect(huffmanEncoding, &HuffmanEncoding::started, this, [this] {
        ui->progressBar->setHidden(false);
    });

    connect(huffmanEncoding, &HuffmanEncoding::finished, this, [this] {
        ui->progressBar->setHidden(true);
    });

    connect(huffmanEncoding, SIGNAL(progressChanged(QString)),ui->textEdit, SLOT(append(QString)));
    connect(huffmanEncoding, SIGNAL(progressCounted(int)), ui->progressBar, SLOT(setValue(int)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->clearListButton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->embedButton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->addFilesButton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->listWidget, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->saveLocation, SLOT(setEnabled(bool)));
}
void CompressWindow::showEvent(QShowEvent *)
{
    if(!huffmanEncoding->isRunning()) {
        ui->progressBar->setHidden(true);
        ui->progressBar->setValue(0);
        ui->textEdit->clear();
        ui->listWidget->clear();
        ui->saveLocation->clear();
    }
}
CompressWindow::~CompressWindow()
{
    delete ui;
    delete huffmanEncoding;
}

void CompressWindow::disabledButton()
{
    ui->clearListButton->setDisabled(true);
    ui->embedButton->setDisabled(true);
    ui->addFilesButton->setDisabled(true);
    ui->listWidget->setDisabled(true);
    ui->saveLocation->setDisabled(true);
}

void CompressWindow::on_addFilesButton_clicked()
{
    ui->progressBar->setValue(0);
    //Select Files
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Files to Compress",
                "C://",
                "All Files (*.*)");
    QStringList list = files;
    QStringList::Iterator it = list.begin();
    //Proccessing List
    if (it==list.end()) //Cancel or no files
    {
        ui->textEdit->setText("Operation Canceled.");
    } else
    {
        while(it != list.end()) {
            ui->listWidget->addItem(*it);
            ++it;
        }
    }
}

void CompressWindow::on_saveAsButton_clicked()
{
    QString outputfile = QFileDialog::getSaveFileName(this,
                                                      tr("Save As"),
                                                      QString(),
                                                      tr("Frog File (*.frog)"));
    if (outputfile.isNull() || outputfile.isEmpty()) // Cancel
    {
        ui->textEdit->setText("Operation Canceled."); //Cancel operation
    } else //get filename to save
    {
        if (!outputfile.endsWith(".frog")) {
            outputfile.append(".frog");
        }
        ui->saveLocation->setText(outputfile);
    }
}

void CompressWindow::on_clearListButton_clicked()
{
    ui->listWidget->clear();
}

void CompressWindow::on_embedButton_clicked()
{
    if (ui->listWidget->count() > 0 && ui->saveLocation->text().length() > 0) {
        // Disabled Button
        disabledButton();

        //Init Compressor
        int64_t totalsize;
        totalsize=0;

        ui->textEdit->setText("Preparing File : ");

        QStringList list;
        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            totalsize=totalsize+QFileInfo(item->text()).size();
            list.append(item->text());
            ui->textEdit->append(item->text());
            //Do stuff!
        }

        //Starting
        ui->textEdit->append("Compressing...");
        huffmanEncoding->setInputFile(list);
        huffmanEncoding->setOutputFile(ui->saveLocation->text());
        huffmanEncoding->setTotalSize(totalsize);
        huffmanEncoding->start();
    } else {
        ui->textEdit->setText("Please fill all requirement");
    }
}
