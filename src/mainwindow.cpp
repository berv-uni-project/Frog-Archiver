#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    huffmanEncoding = new HuffmanEncoding();
    huffmanDecoding = new HuffmanDecoding();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete huffmanEncoding;
    delete huffmanDecoding;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    About Ab1;
    Ab1.setModal(true);
    Ab1.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    Extract();
}

void MainWindow::on_textEdit_textChanged()
{

}

void MainWindow::on_pushButton_6_clicked()
{
    Compress();
}

void MainWindow::on_actionCompress_triggered()
{
    Compress();
}

void MainWindow::Extract()
{
    ui->progressBar->setValue(0);
    QString filename=QFileDialog::getOpenFileName (
                this,
                tr("Open File to Extract..."),
                "C://",
                "Frog File (*.frog)"
                );
    if (filename=="") //Cancel Open File
    {
        ui->textEdit->setText("Operation Canceled");
    } else //Open File
    {
        QString outputdir = QFileDialog::getExistingDirectory(this,tr("Choose Directory to Extract"),
                                                              "C:\\");
        if (outputdir=="")
        {
            ui->textEdit->setText("Operation Canceled");
        } else
        {
            ui->textEdit->setText("");
            connect(huffmanDecoding, SIGNAL(progressChanged(QString)),ui->textEdit, SLOT(append(QString)));
            connect(huffmanDecoding, SIGNAL(progressCounted(int)), ui->progressBar, SLOT(setValue(int)));
            huffmanDecoding->setInputFile(filename);
            huffmanDecoding->setOutputFile(outputdir);
            huffmanDecoding->start();
        }
    }
}

void MainWindow::Compress()
{
    //init Compress
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
        //Choose save file
        QString outputfile = QFileDialog::getSaveFileName(this,
                                                  tr("Location to Compress")
                                                  ,"C:\\",tr("Frog File (*.frog)"));
        if (outputfile=="") // Cancel
        {
            ui->textEdit->setText("Operation Canceled."); //Cancel operation
        } else //get filename to save
        {
            //Init Compressor
            int64_t totalsize;
            totalsize=0;

            //read total files size
            ui->textEdit->setText("Preparing File : ");
            while (it!=list.end()) {
                //Total size
                totalsize=totalsize+QFileInfo(*it).size();
                ui->textEdit->append(*it);
                ++it;
            }

            //Starting
            ui->textEdit->append("Compressing...");
            connect(huffmanEncoding, SIGNAL(progressChanged(QString)),ui->textEdit, SLOT(append(QString)));
            connect(huffmanEncoding, SIGNAL(progressCounted(int)), ui->progressBar, SLOT(setValue(int)));
            huffmanEncoding->setInputFile(list);
            huffmanEncoding->setOutputFile(outputfile);
            huffmanEncoding->setTotalSize(totalsize);
            huffmanEncoding->start();
        }
    }
}

void MainWindow::on_actionExtract_triggered()
{
    Extract();
}

void MainWindow::on_actionCompressButton_triggered()
{
    Compress();
}

void MainWindow::on_actionExtractButton_triggered()
{
    Extract();
}
