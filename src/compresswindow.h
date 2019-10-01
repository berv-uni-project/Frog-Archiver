#ifndef COMPRESSWINDOW_H
#define COMPRESSWINDOW_H

#include "huffmanencoding.h"

#include <QDialog>

namespace Ui {
class CompressWindow;
}

class CompressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CompressWindow(QWidget *parent = nullptr);
    ~CompressWindow();


private slots:
    void on_addFilesButton_clicked();

    void on_saveAsButton_clicked();

    void on_clearListButton_clicked();

    void on_embedButton_clicked();
protected:
    void showEvent(QShowEvent *) override;
private:
    Ui::CompressWindow *ui;
    HuffmanEncoding *huffmanEncoding;
    void disabledButton();
};

#endif // COMPRESSWINDOW_H
