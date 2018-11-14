#ifndef DIALOGEMBED_H
#define DIALOGEMBED_H

#include <QDialog>
#include "huffmanencoding.h"

namespace Ui {
class DialogEmbed;
}

class DialogEmbed : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEmbed(QWidget *parent = nullptr);
    ~DialogEmbed();

private slots:
    void on_addFilesBUtton_clicked();

    void on_saveAsButton_clicked();

    void on_clearButton_clicked();

    void on_embedButton_clicked();

private:
    Ui::DialogEmbed *ui;
    HuffmanEncoding *huffmanEncoding;
    void disabledButton();
};

#endif // DIALOGEMBED_H
