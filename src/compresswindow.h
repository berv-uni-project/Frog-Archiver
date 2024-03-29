#pragma once

#include "huffmanencoding.h"

#include <QDialog>

namespace Ui {
class CompressWindow;
}

class CompressWindow : public QDialog {
  Q_OBJECT

public:
  explicit CompressWindow(QWidget *parent = nullptr);
  ~CompressWindow();

private slots:
  void addFiles();
  void saveAs();
  void compress();

protected:
  void showEvent(QShowEvent *) override;

private:
  Ui::CompressWindow *ui;
  HuffmanEncoding *huffmanEncoding;
  void disabledButton();
};
