#include "license.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogembed.h"
#include "extractwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
    ui->buttonCompress->setIconSize(iconSize);
    ui->buttonExtract->setIconSize(iconSize);
    extractWindow = new ExtractWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete extractWindow;
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

void MainWindow::on_actionCompress_triggered()
{
    Compress();
}

void MainWindow::Extract()
{
    extractWindow->show();
    this->hide();
}

void MainWindow::Compress()
{
    DialogEmbed dialogEmbed;
    dialogEmbed.setModal(true);
    dialogEmbed.setWindowFlags(Qt::Window);
    dialogEmbed.exec();
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

void MainWindow::on_actionLicense_triggered()
{
    License license;
    license.setModal(true);
    license.exec();
}

void MainWindow::on_buttonExtract_clicked()
{
    Extract();
}

void MainWindow::on_buttonCompress_clicked()
{
    Compress();
}

