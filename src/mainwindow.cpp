#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
            QTime timer;
            Huffman H;
            timer.start();
            H.decoder(filename,outputdir,ui->textEdit);
            int time = timer.elapsed();
            ui->textEdit->append("Result : ");
            ui->textEdit->append(QString("File Resource : %1").arg(filename));
            ui->textEdit->append(QString("Destination directory : %1").arg(outputdir));
            QFileInfo temp(filename);
            ui->textEdit->append(QString("Resource Size : %1 byte").arg(temp.size()));
            ui->textEdit->append(QString("Total Extracted Size : %1 byte").arg(H.tempsize));
            ui->textEdit->append(QString("Time elapsed : %1 ms").arg(time));
            ui->progressBar->setValue(100);
            ui->textEdit->append("Extract Success");
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
            QTime TheTime;
            Huffman H;
            ui->textEdit->append("Compressing...");
            TheTime.start();
            H.encoder(files,outputfile,ui->progressBar,ui->textEdit,totalsize);
            int time = TheTime.elapsed();
            ui->textEdit->append("-----------------------------------------------------");
            ui->textEdit->append("Result : ");
            ui->textEdit->append(QString("Output File : %1").arg(outputfile));
            ui->textEdit->append(QString("Time Elapsed : %1 ms").arg(time));
            ui->textEdit->append(QString("Total File Size Process : %1 byte").arg(totalsize));
            int64_t compresssize = QFileInfo(outputfile).size();
            ui->textEdit->append(QString("Compressed File Size : %1 byte").arg(compresssize));
            ui->textEdit->append(QString("Compress Ratio : %1%").arg(100*compresssize/totalsize));
            QList<double>::iterator il = H.ratat.begin();
            int i = 1;
            while(il!=H.ratat.end())
            {
                ui->textEdit->append(QString("Rata-rata bit/simbol file %1 : %2").arg(i).arg(*il));
                        i++;
                ++il;
            }
            QList<double>::iterator ih = H.entropit.begin();
            i = 1;
            while(ih!=H.entropit.end())
            {
                ui->textEdit->append(QString("Entropi file %1 : %2").arg(i).arg(*ih));
                        i++;
                ++ih;
            }
            //Finish

            ui->progressBar->setValue(100);

            ui->textEdit->append("Compress Success");
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
