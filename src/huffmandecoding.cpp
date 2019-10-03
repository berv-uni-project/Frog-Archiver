#include "huffmandecoding.h"

/**
 * Empty constructor 
 */
HuffmanDecoding::HuffmanDecoding()
{
    // Empty constructor
}

/**
 * Reader, to read file into end of file 
 * @param infile stream of files to be read
 * @param bit_pos last position of reader
 * @param temp_char a char for checking position
 * @return a char in a position
 */
unsigned char HuffmanDecoding::huffman_read(ifstream &infile, int &bit_pos, unsigned char &temp_char)
{
    unsigned char c;
    c = (temp_char >> (7 - bit_pos)) % 2; //get bit from byte
    ++bit_pos;
    bit_pos = bit_pos % 8;

    if (bit_pos == 0)
    {
        if (!infile.eof())
        { //bisa aja gak ketemu eof di file yang bukan terakhir soalnya multifile
            temp_char = infile.get();
        }
        else
        {
            c = 2;
            bit_pos = 0; //byte block (0-7)
            temp_char = infile.get();
        }
    }

    return c;
}

/**
 * decoder is main process to do decompressing
 * there is also event emitter
 */ 
void HuffmanDecoding::decoder()
{
    QTime timer;
    timer.start();
    tempsize = 0;
    activefile = "";

    string ifile = ifi.toStdString();

    //membuka input file;
    ifstream infile(ifile.c_str(), ios::in | ios::binary);

    if (!infile)
    {
        exit(1);
    }

    char cc;
    int number_of_file = infile.get();
    int proccess = 0;
    while (infile.get(cc))
    {
        //loop until eof in decoded file

        string ofile = directory.toStdString();
        ofile.append("/");
        if (cc == '\\')
        {
            infile.get(cc);
        }

        while (cc != '\\')
        {
            ofile += cc;
            infile.get(cc);
        }

        emit progressChanged(QString::fromStdString(ofile));
        //prepare output file
        ofstream outfile(ofile.c_str(), ios::out | ios::binary);

        //simpan frequensi dari input file
        unsigned int f[256];
        char c;
        unsigned char ch;
        unsigned int j = 1;

        for (int i = 0; i < 256; ++i)
        {
            //baca dan ubah 4 byte ke 32 bit;
            f[i] = 0;
            for (int k = 3; k >= 0; --k)
            {
                infile.get(c);
                ch = c;
                f[i] = f[i] + (ch * (j << (8 * k)));
            }
        }

        //re-construct the Huffman tree
        Queue<Tree> q(3); //use a 3-(priority)heap (again)
        Tree *t1;
        Tree *t2;
        Tree *t_temp;

        for (int i = 0; i < 256; ++i)
        {
            if (f[i] > 0)
            {
                //send freq-char pairs to the priority heap as Huffman trees
                t1 = new Tree;
                (*t1).set_freq(f[i]);
                (*t1).set_char(static_cast<unsigned char>(i));
                q.push(t1);
            }
        }

        //construct the main Huffman Tree (as in Encoder func.)

        do
        {
            t1 = q.pop();
            if (!q.isEmpty())
            {
                //get the 2 lowest freq. H. trees and combine them into one
                //and put back into the priority heap
                t2 = q.pop();
                t_temp = new Tree;
                (*t_temp).set_freq((*t1).get_freq() + (*t2).get_freq());
                (*t_temp).set_left((*t1).get_root());
                (*t_temp).set_right((*t2).get_root());
                q.push(t_temp);
            }
        } while (!q.isEmpty()); //until all sub-trees combined into one

        //baca huffman string dari input file;
        //ubah ke char dan tulis di output file;
        string str;
        unsigned char ch2;
        unsigned int total_chars = (*t1).get_freq();
        int bit = 0;
        unsigned char cs = infile.get();
        while (total_chars > 0)
        {
            str = ""; //current Huffman string
            do
            {
                //baca huffman string bit per bit
                ch = huffman_read(infile, bit, cs);
                if (ch == 0)
                {
                    str = str + '0';
                }
                else if (ch == 1)
                {
                    str = str + '1';
                }
            } while (!(*t1).get_huf_char(str, ch2));
            //continue until a char is found

            //tuliskan char ke output file
            outfile.put(static_cast<char>(ch2));
            total_chars -= 1;
        }

        outfile.close();
        QString outfilename;
        outfilename = QString::fromStdString(ofile);
        QFileInfo temp(outfilename);
        tempsize = tempsize + temp.size();
        emit progressChanged(QString("Extract %1 finished").arg(outfilename));
        emit progressChanged(QString("Size : %1 byte ").arg(temp.size()));
        emit progressCounted(100 * proccess / number_of_file);
        proccess += 1;
    }
    //end of read decode file
    infile.close();
    int time = timer.elapsed();
    emit progressChanged("Result : ");
    emit progressChanged(QString("File Resource : %1").arg(ifi));
    emit progressChanged(QString("Destination directory : %1").arg(directory));
    QFileInfo temp(ifi);
    emit progressChanged(QString("Resource Size : %1 byte").arg(temp.size()));
    emit progressChanged(QString("Total Extracted Size : %1 byte").arg(tempsize));
    emit progressChanged(QString("Time elapsed : %1 ms").arg(time));
    emit progressCounted(100);
    emit progressChanged("Extract Success");
    emit setEnabled(true);
}
