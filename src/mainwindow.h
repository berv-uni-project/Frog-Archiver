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
#include <QElapsedTimer>
#include <stdint.h>
#include <QSettings>

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

protected:
  void changeEvent(QEvent *e);
private slots:
  void showAbout();
  void showLicense();
  void changeLanguage(QAction *);

private:
  void detectTranslations();
  QSettings * m_settings;
  QMap<QString, QList<QTranslator *>> m_translations;
  Ui::MainWindow *ui;
  CompressWindow *compressWindow = nullptr;
  ExtractWindow *extractWindow = nullptr;
};

