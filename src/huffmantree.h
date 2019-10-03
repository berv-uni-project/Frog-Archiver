#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

class Tree
{
private:
    class Node
    {
    public:
        unsigned int freq;
        unsigned char ch;
        Node *left, *right;

        Node()
        {
            freq = 0;
            ch = '\0';
            left = NULL;
            right = NULL;
        }
    };

    Node *root;

public:
    Tree()
    {
        Node *N = new Node;
        root = N;
    } //ctor
    //Tree(const Tree&);      //cctor
    ~Tree()
    {
        destroy(root);
    } //dtor
    const Tree &operator=(const Tree &);

    friend void decoder();
    friend void encoder();

    void destroy(Node *N)
    {
        if (N)
        {
            destroy(N->left);
            destroy(N->right);

            delete N;
        }
    } //destroy the tree

    unsigned int get_freq() const
    {
        return root->freq;
    }

    unsigned char get_char() const
    {
        return root->ch;
    }

    void set_freq(unsigned int f)
    {
        root->freq = f;
    }

    void set_char(unsigned char c)
    {
        root->ch = c;
    }

    /**
     * Getter. Get root of tree.
     */
    Node *get_root() const
    {
        return root;
    }

    /**
     * getter. Getting left of tree.
     */
    Node *get_left() const
    {
        return root->left;
    }

    /**
     * Getter. Getting right of tree.
     */
    Node *get_right() const
    {
        return root->right;
    }

    /**
     * Setter.
     * Setting another pointer into left a tree
     * @param N another tree
     */
    void set_left(Node *N)
    {
        root->left = N;
    }

    /**
     * Setter.
     * Setting another pointer into right a tree
     * @param N another tree
     */
    void set_right(Node *N)
    {
        root->right = N;
    }

    // operator overloading

    /**
     * Override an operator of ==
     * @param T another tree to be differentiate
     */
    bool operator==(const Tree &T) const
    {
        return (root->freq == T.root->freq);
    }

    /**
     * Override an operator of !=
     * @param T another tree to be differentiate
     */
    bool operator!=(const Tree &T) const
    {
        return (root->freq != T.root->freq);
    }

    /**
     * Override an operator of <
     * @param T another tree to be differentiate
     */
    bool operator<(const Tree &T) const
    {
        return (root->freq < T.root->freq);
    }

    /**
     * Override an operator of <=
     * @param T another tree to be differentiate
     */
    bool operator<=(const Tree &T) const
    {
        return (root->freq <= T.root->freq);
    }
    /**
     * Override an operator of >
     * @param T another tree to be differentiate
     */
    bool operator>(const Tree &T) const
    {
        return (root->freq > T.root->freq);
    }

    /**
     * Override an operator of >=
     * @param T another tree to be differentiate
     */
    bool operator>=(const Tree &T) const
    {
        return (root->freq >= T.root->freq);
    }

    /**
     * get Huffman string of a char.
     * @param N huffman list.
     * @param c a char will be placed
     * @param str kind of string to be replaced
     * @param s output into a string that already placed
     */
    void huffman(Node *N, unsigned char c, string str, string &s) const
    {
        if (N)
        {
            if (!N->left && !N->right && N->ch == c)
            {
                s = str;
            }
            else
            {
                huffman(N->left, c, str + "0", s);
                huffman(N->right, c, str + "1", s);
            }
        }
    }

    /**
     * Print menjadi huffman list.
     * outputs the Huffman char-string pairs list
     * @param N huffman list
     * @param str based to be printed
     */ 
    void huffman_list(Node *N, string str) const
    {
        if (N)
        {
            if (!N->left && !N->right)
            {                                                                      //daun
                cout << print_char(N) << " : " << N->freq << " : " << str << endl; //basis
            }
            else
            {
                //rekursif
                huffman_list(N->left, str + "0");
                huffman_list(N->right, str + "1");
            }
        }
    }

    /**
     * Checking a character is in a tree.
     * To get char equivalent of a Huffman string jika ada.
     * @param str respective tree
     * @param c a char to be finded
     * @return indicate found or not
     */
    bool get_huf_char(string str, unsigned char &c) const
    {
        Node *curr = root;

        for (unsigned i = 0; i < str.size(); ++i)
        {
            if (str[i] == '0')
            {
                curr = curr->left;
            }
            if (str[i] == '1')
            {
                curr = curr->right;
            }
        }

        bool found = false;

        if (!curr->left && !curr->right)
        { //daun
            found = true;
            c = curr->ch;
        }

        return found;
    }

    //prints chars
    /**
     * Printing huffman tree
     * @param N main root of huffman tree
     * @return string of tree structure
     */ 
    string print_char(Node *N) const
    {
        string str = "";

        if (!N->left && !N->right)
        {
            unsigned char c = N->ch;

            if (iscntrl(c) || c == 32)
            {
                char *temp_char = new char;
                for (int i = 0; i < 3; ++i)
                {
                    sprintf(temp_char, "%i", c % 8);
                    c = c - c % 8;
                    c = c / 8;
                    str = (*temp_char) + str;
                }
                str = '/' + str;
            }
            else
            {
                str = c;
            }
        }

        return str;
    }
};

#endif // HUFFMANTREE_H
