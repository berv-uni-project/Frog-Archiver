#ifndef HUFFMANDECODING_H
#define HUFFMANDECODING_H

#include "huffmantree.h"
#include "prioqueue.h"

#include <QFileInfo>
#include <QThread>
#include <QTime>
#include <fstream>

using namespace std;

class HuffmanDecoding : public QThread
{
    Q_OBJECT

public:
    HuffmanDecoding();

    void setInputFile(QString file) {
        ifi = file;
    }

    void setOutputFile(QString output) {
        directory = output;
    }

    void decoder();

    void run() {
        decoder();
    }

private:
    QList<double> ratat;
    QList<double> entropit;

    int64_t tempsize;
    QString activefile;

    QString ifi;
    QString directory;

private:
    unsigned char huffman_read(ifstream& infile, int &bit_pos,unsigned char &temp_char);


signals:
    void progressChanged(QString info);
    void progressCounted(int progress);
    void setEnabled(bool enable);

};

#endif // HUFFMANDECODING_H
