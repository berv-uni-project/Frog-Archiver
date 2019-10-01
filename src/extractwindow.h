#ifndef EXTRACTWINDOW_H
#define EXTRACTWINDOW_H

#include "huffmandecoding.h"

#include <QDialog>

namespace Ui {
class ExtractWindow;
}

class ExtractWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractWindow(QWidget *parent = nullptr);
    ~ExtractWindow() override;
    void Extract();

private slots:
    void on_inputButton_clicked();

    void on_outputDirButton_clicked();

    void on_extractButton_clicked();
protected:
    void showEvent(QShowEvent *) override;

private:
    Ui::ExtractWindow *ui;
    HuffmanDecoding *huffmanDecoding;
};

#endif // EXTRACTWINDOW_H
