#ifndef HUFFMANENCODING_H
#define HUFFMANENCODING_H

#include <QThread>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class HuffmanEncoding: public QThread
{
    Q_OBJECT
public:
    HuffmanEncoding();
    static int bit_pos; //byte block (0-7)
    static unsigned char temp_char;

    void setInputFile(QStringList input) {
        inf = input;
    }

    void setOutputFile(QString output) {
        of = output;
    }

    void setTotalSize(int64_t totalsizeInput) {
        totalsize = totalsizeInput;
    }

    void encoder();

    void run() {
        encoder();
    }

private:
    QStringList inf;
    QString of;
    int64_t totalsize;
    QList<double> ratat;
    QList<double> entropit;

    int64_t tempsize;
    QString activefile;

    void huffmanWrite(unsigned char c, ofstream& outfile) {
        if (c < 2) {
            if (c==1){
                temp_char = temp_char | (c<<(7-bit_pos)); //add 1 to the byte
            } else {
                temp_char = temp_char & static_cast<unsigned char> (255-(1<<(7-bit_pos))); //add 0 to the byte
            }
            ++bit_pos;
            bit_pos %= 8;
            if (bit_pos==0) {
                outfile.put(temp_char);
                temp_char = '\0';
            }
        } else {
            if(bit_pos!=0){
                outfile.put(temp_char);
            }
            temp_char = '\0';
            bit_pos = 0;
        }
    }

signals:
    void progressChanged(QString info);
    void progressCounted(int progress);
    void setEnabled(bool enable);
};

#endif // HUFFMANENCODING_H
