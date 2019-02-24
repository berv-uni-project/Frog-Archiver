#ifndef COMPRESSWINDOW_H
#define COMPRESSWINDOW_H

#include "huffmanencoding.h"

#include <QMainWindow>

namespace Ui {
class CompressWindow;
}

class CompressWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CompressWindow(QWidget *parent = nullptr);
    ~CompressWindow();


private slots:
    void on_addFilesBUtton_clicked();

    void on_saveAsButton_clicked();

    void on_clearButton_clicked();

    void on_embedButton_clicked();

private:
    Ui::CompressWindow *ui;
    HuffmanEncoding *huffmanEncoding;
    void disabledButton();
};

#endif // COMPRESSWINDOW_H
