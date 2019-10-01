#include "license.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    compressWindow = new CompressWindow(this);

    ui->mainToolBar->addAction(ui->actionCompress);
    connect(ui->actionCompress, &QAction::triggered, this, &MainWindow::Compress);
    connect(ui->buttonCompress, &QPushButton::pressed, this, &MainWindow::Compress);

    ui->mainToolBar->addAction(ui->actionExtract);
    connect(ui->actionExtract, &QAction::triggered, this, &MainWindow::Extract);
    connect(ui->buttonExtract, &QPushButton::pressed, this, &MainWindow::Extract);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete extractWindow;
    delete compressWindow;
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

void MainWindow::Extract()
{
    extractWindow->show();
    this->hide();
}

void MainWindow::Compress()
{
    compressWindow->show();
    this->hide();
}

void MainWindow::on_actionLicense_triggered()
{
    License license;
    license.setModal(true);
    license.exec();
}
