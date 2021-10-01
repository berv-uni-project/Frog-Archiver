#include "mainwindow.h"
#include "extractwindow.h"
#include "license.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      compressWindow(new CompressWindow(this)),
      extractWindow(new ExtractWindow(this)) {
  ui->setupUi(this);
  QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
  ui->buttonCompress->setIconSize(iconSize);
  ui->buttonExtract->setIconSize(iconSize);

  ui->mainToolBar->addAction(ui->actionCompress);
  connect(ui->actionCompress, &QAction::triggered, this, &MainWindow::Compress);
  connect(ui->buttonCompress, &QPushButton::pressed, this,
          &MainWindow::Compress);

  ui->mainToolBar->addAction(ui->actionExtract);
  connect(ui->actionExtract, &QAction::triggered, this, &MainWindow::Extract);
  connect(ui->buttonExtract, &QPushButton::pressed, this, &MainWindow::Extract);

  connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
  connect(ui->actionLicense, &QAction::triggered, this, &MainWindow::showLicense);
}

MainWindow::~MainWindow() {
  delete compressWindow;
  delete extractWindow;
  delete ui;
}

void MainWindow::showAbout() {
  About Ab1;
  Ab1.setModal(true);
  Ab1.exec();
}

void MainWindow::Extract() { extractWindow->show(); }

void MainWindow::Compress() { compressWindow->show(); }

void MainWindow::showLicense() {
  License license;
  license.setModal(true);
  license.exec();
}
