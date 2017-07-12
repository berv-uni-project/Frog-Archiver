#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <cctype>
#include <stdlib.h>
#include <math.h>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QProgressBar>
#include <QTextEdit>
#include <QThread>
#include "prioqueue.h"
#include "huffmantree.h"

using namespace std;

/////////////////// HUFFMAN ////////////////

void inline huffman_write(unsigned char c, ofstream& outfile) {
    static int bit_pos = 0; //byte block (0-7)
    static unsigned char temp_char = '\0';

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

           // cout<<temp_char<<" = "<<(int)temp_char<<endl;

            temp_char = '\0';
        }
    } else {
        if(bit_pos!=0){
            outfile.put(temp_char);
        //	cout<<temp_char<<" = "<<(int)temp_char<<endl;
        }
        temp_char = '\0';
        bit_pos = 0;
        //buat apa??
    }
}

unsigned inline char huffman_read(ifstream& infile,int &bit_pos,unsigned char &temp_char){
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

class Huffman {
public :
    //Komponen
    QList<double> ratat;
    QList<double> entropit;

    int64_t tempsize;
    QString activefile;

    //Method Utama
    void encoder (QStringList inf,QString of,QProgressBar *Bar,QTextEdit *Text,int64_t totalsize) {
        tempsize = 0;
        activefile = "";

        //Setting output file
        string ofile = of.toStdString();
        ofstream outfile(ofile.c_str(), ios::out|ios::binary);
        if(!outfile) {
            exit(1);
        }

        //loop many file
        QStringList::iterator it = inf.begin();
        while(it!=inf.end())
        {
            //Setting input file
            string ifile = (*it).toStdString();
            ifstream infile (ifile.c_str(), ios::in | ios::binary);
            if (!infile) {
                exit(1);
            }

            //initialisasi frekuensi kemunculan huruf
            unsigned int f[256];
            for (int i=0; i<256; ++i) {
                f[i] = 0;
            }

            //hitung kemunculan huruf
            char c;
            unsigned char ch;
            while(infile.get(c)) {
                ch = c;
                ++f[ch];
            }

            infile.clear();
            infile.seekg(0);

            Queue<Tree> q(3);

            Tree* t1;
            Tree* t2;
            Tree* t_temp;

            QFileInfo NF(*it);
            QString temp = NF.fileName();
            string innfile= temp.toStdString();

            //write file name in header
            for (unsigned int i=0; i<innfile.size(); ++i) {
                outfile.put(innfile[i]);
            }
                outfile.put('\n');

            for (int i=0; i<256; ++i) {
                //output char freq table to the output file
                //ubah 32 bit jadi 4 byte
                outfile.put(static_cast<unsigned char>(f[i]>>24));
                outfile.put(static_cast<unsigned char>((f[i]>>16)%256));
                outfile.put(static_cast<unsigned char>((f[i]>>8)%256));
                outfile.put(static_cast<unsigned char>(f[i]%256));

                if(f[i]>0)
                {
                    //send freq-char pairs to the priority heap as Huffman trees
                    t1=new Tree;
                    (*t1).set_freq(f[i]);
                    (*t1).set_char(static_cast<unsigned char>(i));
                    q.push(t1);
                }
            }

            do {
                t1 = q.pop();
                if (!q.isEmpty()) {
                    t2 = q.pop();
                    t_temp = new Tree;
                    (*t_temp).set_freq((*t1).get_freq()+(*t2).get_freq());
                    (*t_temp).set_left((*t1).get_root());
                    (*t_temp).set_right((*t2).get_root());
                    q.push(t_temp);
                }
            } while (!q.isEmpty());

            //simpan huffman string ke H_table
            unsigned int total_chars=(*t1).get_freq();
            string H_table[256];
            //initialisasi perhitungan entropi
            double sum=0, peluang=0,entropi=0;
            unsigned char uc;
            for(unsigned short us=0; us<256; ++us) {
                H_table[us]="";
                if(f[us]>0){
                    uc=static_cast<unsigned char>(us);
                    (*t1).huffman((*t1).get_root(), uc, "", H_table[us]);
                    sum += H_table[us].size()*f[us];
                    peluang = (double) f[us] / total_chars;
                    entropi += ((-1)*peluang*log2(peluang));
                }
            }
            entropit.push_back(entropi);

            double rata = (double) sum / total_chars;
            ratat.push_back(rata);

            unsigned char ch2;
            while(infile.get(c))
            {
                ch=c;
                //tulis huffman string ke file output bit per bit
                for(unsigned int i=0; i<H_table[ch].size(); ++i) {
                    if(H_table[ch].at(i)=='0') {
                        ch2=0;
                    } else if(H_table[ch].at(i)=='1') {
                        ch2=1;
                    }
                    huffman_write(ch2, outfile);
                }
            }

            ch2=2; // send EOF
            huffman_write(ch2, outfile);

            //Change Proccess
            QFileInfo thisfile(*it);
            activefile = *it;
            tempsize += thisfile.size();
            int64_t progress = 100*tempsize/totalsize;
            Text->append(QString("%1 ... %2%").arg(*it).arg(progress));
            Bar->setValue(progress);

            infile.close(); //Close opened file for another input
            ++it; //Next file
        } //end of loop many file
        outfile.close();
        //Close outfile
    }


    void decoder(QString ifi,QString directory,QTextEdit *textedit) {
        tempsize = 0;
        activefile = "";

        string ifile = ifi.toStdString();

        //membuka input file;
        ifstream infile(ifile.c_str(), ios::in|ios::binary);

        if(!infile) {
            exit(1);
        }

        char cc;
        while(infile.get(cc)){
        //loop until eof in decoded file

            string ofile=directory.toStdString();
            ofile.append("/");

            while(cc!='.'){
                ofile+=cc;
                infile.get(cc);
            }

            ofile+=cc;

            infile.get(cc);

            while(cc!='\n'){
                ofile+=cc;
                infile.get(cc);
            }

            textedit->append(QString::fromStdString(ofile));
            //prepare output file
            ofstream outfile(ofile.c_str(), ios::out | ios::binary);

            //simpan frequensi dari input file
            unsigned int f[256];
            char c;
            unsigned char ch;
            unsigned int j=1;

            for(int i=0; i<256; ++i){
                //baca dan ubah 4 byte ke 32 bit;
                f[i]=0;
                for(int k=3; k>=0; --k){
                    infile.get(c);
                    ch=c;
                    f[i] = f[i]+ (ch*(j<<(8*k)));
                }
            }

            //re-construct the Huffman tree
            Queue<Tree> q(3); //use a 3-(priority)heap (again)
            Tree* t1;
            Tree* t2;
            Tree* t_temp;

            for(int i=0; i<256; ++i) {
                if(f[i]>0){
                    //send freq-char pairs to the priority heap as Huffman trees
                    t1=new Tree;
                    (*t1).set_freq(f[i]);
                    (*t1).set_char(static_cast<unsigned char>(i));
                    q.push(t1);
                }
            }

            //construct the main Huffman Tree (as in Encoder func.)

            do{
                t1=q.pop();
                if(!q.isEmpty()){
                    //get the 2 lowest freq. H. trees and combine them into one
                    //and put back into the priority heap
                    t2 = q.pop();
                    t_temp = new Tree;
                    (*t_temp).set_freq((*t1).get_freq()+(*t2).get_freq());
                    (*t_temp).set_left((*t1).get_root());
                    (*t_temp).set_right((*t2).get_root());
                    q.push(t_temp);
                }
            } while(!q.isEmpty()); //until all sub-trees combined into one

            QThread::msleep(200);
            //baca huffman string dari input file;
            //ubah ke char dan tulis di output file;
            string str;
            unsigned char ch2;
            unsigned int total_chars=(*t1).get_freq();
            int bit=0;
            unsigned char cs= infile.get();
            while(total_chars>0){
                str=""; //current Huffman string
                do {
                    //baca huffman string bit per bit
                    ch=huffman_read(infile,bit,cs);
                    if(ch==0){
                        str = str + '0';
                    } else if(ch==1){
                        str = str + '1';
                    }
                } while(!(*t1).get_huf_char(str, ch2));
                //continue until a char is found

                //tuliskan char ke output file
                outfile.put(static_cast<char>(ch2));
                --total_chars;
            }

            outfile.close();
            QString outfilename;
            outfilename = QString::fromStdString(ofile);
            QFileInfo temp(outfilename);
            tempsize = tempsize+temp.size();
            textedit->append(QString("Compress %1 finished").arg(outfilename));
            textedit->append(QString("Size : %1 byte ").arg(temp.size()));
        }
        //end of read decode file
        infile.close();
    }
};

#endif // HUFFMAN_H
