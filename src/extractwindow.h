#ifndef EXTRACTWINDOW_H
#define EXTRACTWINDOW_H

#include "huffmandecoding.h"

#include <QMainWindow>

namespace Ui {
class ExtractWindow;
}

class ExtractWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExtractWindow(QWidget *parent = nullptr);
    ~ExtractWindow();
    void Extract();

private slots:
    void on_inputButton_clicked();

    void on_outputDirButton_clicked();

    void on_extractButton_clicked();

private:
    Ui::ExtractWindow *ui;
    HuffmanDecoding *huffmanDecoding;
};

#endif // EXTRACTWINDOW_H
