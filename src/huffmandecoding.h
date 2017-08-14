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
    unsigned char huffman_read(ifstream& infile,int &bit_pos,unsigned char &temp_char){
        //static int bit_pos = 0;  //byte block (0-7)
        //static unsigned char temp_char = infile.get();

        unsigned char c;

        c = (temp_char >> (7-bit_pos)) % 2; //get bit from byte
        ++bit_pos;
        bit_pos = bit_pos % 8;

        if(bit_pos == 0) {
            if (!infile.eof()){  //bisa aja gak ketemu eof di file yang bukan terakhir soalnya multifile
                temp_char = infile.get();
            } else {
                c = 2;
                bit_pos = 0;  //byte block (0-7)
                temp_char = infile.get();
            }
        }

        return c;
    }


signals:
    void progressChanged(QString info);
    void progressCounted(int progress);


};

#endif // HUFFMANDECODING_H
