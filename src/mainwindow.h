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
#include <stdint.h>
#include "about.h"
#include "huffmandecoding.h"
#include "huffmanencoding.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void Compress();
    void Extract();

    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_4_clicked();

    void on_textEdit_textChanged();

    void on_pushButton_6_clicked();

    void on_actionCompress_triggered();

    void on_actionExtract_triggered();

    void on_actionCompressButton_triggered();

    void on_actionExtractButton_triggered();

private:
    Ui::MainWindow *ui;
    About *aboutwindow;
    HuffmanEncoding *huffmanEncoding;
    HuffmanDecoding *huffmanDecoding;
};

#endif // MAINWINDOW_H
