#include "huffmanencoding.h"
#include "huffmantree.h"
#include "prioqueue.h"

#include <QFileInfo>
#include <QTime>
#include <cmath>

using namespace std;

int HuffmanEncoding::bit_pos = 0;
unsigned char HuffmanEncoding::temp_char = '\0';

/**
 * Emptry constructor 
 */
HuffmanEncoding::HuffmanEncoding()
{
    // empty constructor
}

/**
 * Utillity to write a file 
 * @param c character to be written in a file
 * @param outfile file streaming to be written
 */
void HuffmanEncoding::huffmanWrite(unsigned char c, ofstream &outfile)
{
    if (c < 2)
    {
        if (c == 1)
        {
            temp_char = temp_char | (c << (7 - bit_pos)); //add 1 to the byte
        }
        else
        {
            temp_char = temp_char & static_cast<unsigned char>(255 - (1 << (7 - bit_pos))); //add 0 to the byte
        }
        ++bit_pos;
        bit_pos %= 8;
        if (bit_pos == 0)
        {
            outfile.put(temp_char);
            temp_char = '\0';
        }
    }
    else
    {
        if (bit_pos != 0)
        {
            outfile.put(temp_char);
        }
        temp_char = '\0';
        bit_pos = 0;
    }
}

/**
 * Main process to do compressing
 * there is also event emitter
 */
void HuffmanEncoding::encoder()
{
    QTime timer;
    timer.start();
    tempsize = 0;
    activefile = "";

    //Setting output file
    string ofile = of.toStdString();
    ofstream outfile(ofile.c_str(), ios::out | ios::binary);

    if (!outfile)
    {
        exit(1);
    }

    outfile.put(inf.size());

    //loop many file
    QStringList::iterator it = inf.begin();
    while (it != inf.end())
    {
        //Setting input file
        string ifile = (*it).toStdString();
        ifstream infile(ifile.c_str(), ios::in | ios::binary);
        if (!infile)
        {
            exit(1);
        }

        //initialisasi frekuensi kemunculan huruf
        unsigned int f[256];
        for (int i = 0; i < 256; ++i)
        {
            f[i] = 0;
        }

        //hitung kemunculan huruf
        char c;
        unsigned char ch;
        while (infile.get(c))
        {
            ch = c;
            ++f[ch];
        }

        infile.clear();
        infile.seekg(0);

        Queue<Tree> q(3);

        Tree *t1;
        Tree *t2;
        Tree *t_temp;

        QFileInfo NF(*it);
        QString temp = NF.fileName();
        string innfile = temp.toStdString();
        outfile.put('\\');
        //write file name in header
        for (unsigned int i = 0; i < innfile.size(); ++i)
        {
            outfile.put(innfile[i]);
        }
        outfile.put('\\');

        for (int i = 0; i < 256; ++i)
        {
            //output char freq table to the output file
            //ubah 32 bit jadi 4 byte
            outfile.put(static_cast<unsigned char>(f[i] >> 24));
            outfile.put(static_cast<unsigned char>((f[i] >> 16) % 256));
            outfile.put(static_cast<unsigned char>((f[i] >> 8) % 256));
            outfile.put(static_cast<unsigned char>(f[i] % 256));

            if (f[i] > 0)
            {
                //send freq-char pairs to the priority heap as Huffman trees
                t1 = new Tree;
                (*t1).set_freq(f[i]);
                (*t1).set_char(static_cast<unsigned char>(i));
                q.push(t1);
            }
        }

        do
        {
            t1 = q.pop();
            if (!q.isEmpty())
            {
                t2 = q.pop();
                t_temp = new Tree;
                (*t_temp).set_freq((*t1).get_freq() + (*t2).get_freq());
                (*t_temp).set_left((*t1).get_root());
                (*t_temp).set_right((*t2).get_root());
                q.push(t_temp);
            }
        } while (!q.isEmpty());

        //simpan huffman string ke H_table
        unsigned int total_chars = (*t1).get_freq();
        string H_table[256];
        //initialisasi perhitungan entropi
        double sum = 0, peluang = 0, entropi = 0, tempCount = 0;
        unsigned char uc;
        for (unsigned short us = 0; us < 256; ++us)
        {
            H_table[us] = "";
            if (f[us] > 0)
            {
                uc = static_cast<unsigned char>(us);
                (*t1).huffman((*t1).get_root(), uc, "", H_table[us]);
                sum += H_table[us].size() * f[us];
                peluang = (double)f[us] / total_chars;
                tempCount = log2(peluang);
                entropi += ((-1) * peluang * tempCount);
            }
        }
        entropit.push_back(entropi);

        double rata = (double)sum / total_chars;
        ratat.push_back(rata);

        unsigned char ch2;
        while (infile.get(c))
        {
            ch = c;
            //tulis huffman string ke file output bit per bit
            for (unsigned int i = 0; i < H_table[ch].size(); ++i)
            {
                if (H_table[ch].at(i) == '0')
                {
                    ch2 = 0;
                }
                else if (H_table[ch].at(i) == '1')
                {
                    ch2 = 1;
                }
                huffmanWrite(ch2, outfile);
            }
        }

        ch2 = 2; // send EOF
        huffmanWrite(ch2, outfile);

        //Change Proccess
        QFileInfo thisfile(*it);
        activefile = *it;
        tempsize += thisfile.size();
        int64_t progress = 100 * tempsize / totalsize;
        emit progressChanged(QString("%1 ... %2%").arg(*it).arg(progress));
        emit progressCounted(progress);

        infile.close(); //Close opened file for another input
        ++it;           //Next file
    }                   //end of loop many file
    outfile.close();
    //Close outfile
    int time = timer.elapsed();
    emit progressChanged("-----------------------------------------------------");
    emit progressChanged("Result : ");
    emit progressChanged(QString("Output File : %1").arg(of));
    emit progressChanged(QString("Time Elapsed : %1 ms").arg(time));
    emit progressChanged(QString("Total File Size Process : %1 byte").arg(totalsize));
    int64_t compresssize = QFileInfo(of).size();
    emit progressChanged(QString("Compressed File Size : %1 byte").arg(compresssize));
    emit progressChanged(QString("Compress Ratio : %1%").arg(100 * compresssize / totalsize));
    QList<double>::iterator il = ratat.begin();
    int i = 1;
    while (il != ratat.end())
    {
        emit progressChanged(QString("Rata-rata bit/simbol file %1 : %2").arg(i).arg(*il));
        i++;
        ++il;
    }
    QList<double>::iterator ih = entropit.begin();
    i = 1;
    while (ih != entropit.end())
    {
        emit progressChanged(QString("Entropi file %1 : %2").arg(i).arg(*ih));
        i++;
        ++ih;
    }
    emit progressCounted(100);
    emit progressChanged("Compress Success");
    emit setEnabled(true);
}
