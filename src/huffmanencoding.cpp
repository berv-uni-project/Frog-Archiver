#include "huffmanencoding.h"
#include "huffmantree.h"
#include "prioqueue.h"

#include <QFileInfo>
#include <QTime>

HuffmanEncoding::HuffmanEncoding()
{

}

void HuffmanEncoding::encoder() {
    QTime timer;
    timer.start();
    tempsize = 0;
    activefile = "";

    //Setting output file
    string ofile = of.toStdString();
    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
    if (!outfile) {
        exit(1);
    }

    //loop many file
    QStringList::iterator it = inf.begin();
    while (it != inf.end())
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
                huffmanWrite(ch2, outfile);
            }
        }

        ch2=2; // send EOF
        huffmanWrite(ch2, outfile);

        //Change Proccess
        QFileInfo thisfile(*it);
        activefile = *it;
        tempsize += thisfile.size();
        int64_t progress = 100*tempsize/totalsize;
        emit progressChanged(QString("%1 ... %2%").arg(*it).arg(progress));
        emit progressCounted(progress);

        infile.close(); //Close opened file for another input
        ++it; //Next file
    } //end of loop many file
    outfile.close();
    //Close outfile
    int time = timer.elapsed();
}

