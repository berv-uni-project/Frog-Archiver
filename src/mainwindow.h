/*  Nama File : mainwindow.h
    Kelas interaksi utama Compress dan Decompress dengan Pengguna
*/

#pragma once

#include "about.h"
#include "compresswindow.h"
#include "extractwindow.h"
#include "huffmandecoding.h"
#include "huffmanencoding.h"
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QShortcut>
#include <QTextEdit>
#include <QTextStream>
#include <QTime>
#include <stdint.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  void Compress();
  void Extract();

  ~MainWindow();

private slots:
  void on_actionExit_triggered();

  void on_actionAbout_triggered();

  void on_actionLicense_triggered();

private:
  Ui::MainWindow *ui;
  CompressWindow *compressWindow = nullptr;
  ExtractWindow *extractWindow = nullptr;
};

