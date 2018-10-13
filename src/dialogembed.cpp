#include "dialogembed.h"
#include "ui_dialogembed.h"
#include <QFileDialog>
#include <QFile>

DialogEmbed::DialogEmbed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEmbed)
{
    ui->setupUi(this);
    ui->progressBar->setHidden(true);

    huffmanEncoding = new HuffmanEncoding();
    connect(huffmanEncoding, &HuffmanEncoding::started, this, [this] {
        ui->progressBar->setHidden(false);
    });

    connect(huffmanEncoding, &HuffmanEncoding::finished, this, [this] {
        ui->progressBar->setHidden(true);
    });

    connect(huffmanEncoding, SIGNAL(progressChanged(QString)),ui->textEdit, SLOT(append(QString)));
    connect(huffmanEncoding, SIGNAL(progressCounted(int)), ui->progressBar, SLOT(setValue(int)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->clearButton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->embedButton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->addFilesBUtton, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->listWidget, SLOT(setEnabled(bool)));
    connect(huffmanEncoding, SIGNAL(setEnabled(bool)), ui->saveAsButton, SLOT(setEnabled(bool)));
}

DialogEmbed::~DialogEmbed()
{
    delete ui;
}

void DialogEmbed::disabledButton()
{
    ui->clearButton->setDisabled(true);
    ui->embedButton->setDisabled(true);
    ui->addFilesBUtton->setDisabled(true);
    ui->listWidget->setDisabled(true);
    ui->saveAsButton->setDisabled(true);
}

void DialogEmbed::on_addFilesBUtton_clicked()
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

void DialogEmbed::on_saveAsButton_clicked()
{
    QString outputfile = QFileDialog::getSaveFileName(this,
                                                      tr("Location to Compress"),
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
        ui->saveAsText->setText(outputfile);
    }
}

void DialogEmbed::on_clearButton_clicked()
{
    ui->listWidget->clear();
}

void DialogEmbed::on_embedButton_clicked()
{
    if (ui->listWidget->count() > 0 && ui->saveAsText->text().length() > 0) {
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
        huffmanEncoding->setOutputFile(ui->saveAsText->text());
        huffmanEncoding->setTotalSize(totalsize);
        huffmanEncoding->start();
    } else {
        ui->textEdit->setText("Please fill all requirement");
    }
}
