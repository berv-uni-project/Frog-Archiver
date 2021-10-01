#pragma once

#include <QThread>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

class HuffmanEncoding : public QThread {
  Q_OBJECT
public:
  HuffmanEncoding();
  static int bit_pos; // byte block (0-7)
  static unsigned char temp_char;

  void setInputFile(QStringList input) { inf = input; }

  void setOutputFile(QString output) { of = output; }

  void setTotalSize(int64_t totalsizeInput) { totalsize = totalsizeInput; }

  void encoder();

  void run() { encoder(); }

private:
  QStringList inf;
  QString of;
  int64_t totalsize;
  QList<double> ratat;
  QList<double> entropit;

  int64_t tempsize;
  QString activefile;

  void huffmanWrite(unsigned char c, ofstream &outfile);

signals:
  void progressChanged(QString info);
  void progressCounted(int progress);
  void setEnabled(bool enable);
};
