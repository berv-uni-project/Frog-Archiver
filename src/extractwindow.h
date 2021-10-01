#pragma once

#include "huffmandecoding.h"

#include <QDialog>

namespace Ui {
class ExtractWindow;
}

class ExtractWindow : public QDialog {
  Q_OBJECT

public:
  explicit ExtractWindow(QWidget *parent = nullptr);
  ~ExtractWindow() override;
  void Extract();

private slots:
  void pickInputFile();
  void pickOutputPath();

protected:
  void showEvent(QShowEvent *) override;

private:
  Ui::ExtractWindow *ui;
  HuffmanDecoding *huffmanDecoding;
};
