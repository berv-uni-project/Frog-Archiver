/*  Nama File : mainwindow.h
    Kelas interaksi utama Compress dan Decompress dengan Pengguna
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextEdit>
#include <QApplication>
#include <QTime>
#include <QTextStream>
#include <QShortcut>
#include <stdint.h>
#include "about.h"
#include "compresswindow.h"
#include "extractwindow.h"
#include "huffmandecoding.h"
#include "huffmanencoding.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
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

#endif // MAINWINDOW_H
