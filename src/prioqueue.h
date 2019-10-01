#ifndef PRIOQUEUE_H
#define PRIOQUEUE_H

#include <iostream>
#include <stdlib.h>
using namespace std;

/////////// PRIORITY QUEUE //////////////
template <class Type>
class Queue {
public :
    Queue (int d = 2);      //ctor
    ~Queue();               //dtor
    Queue (const Queue&);   //cctor
    const Queue& operator= (const Queue&);

    void push(Type*);
    Type* pop();
    Type* head();

    bool isEmpty() const;
    bool isFull() const;

    void reheapUp(int, int); //fix heap upward
    void reheapDown(int, int); //fix heap downward
    void swap(Type* &, Type* &);

private:
    int tail;   //last element
    Type* *arr;    //dinamic array
    int size;   //current size array
    static const int incSize = 10;
    int D;
};

//constructor to make new Queue
template <class Type>
Queue<Type>::Queue (int d) {
    if (d<2) {
        d = 2;
    }
    D = d;
    tail = 0;
    size = incSize;
    arr = new Type* [size];
}

//destructor to erase array
template <class Type>
Queue<Type>::~Queue(void){
    delete [] arr;
}

//copy constructor
template <class Type>
Queue<Type>::Queue (const Queue&){
}

template <class Type>
const Queue<Type>& Queue<Type>::operator= (const Queue&){
}

//add new element to array
template <class Type>
void Queue<Type>::push(Type* item) {
    if (isFull()) {     //growing
        int temp_size = size + incSize;
        Type* *temp_arr = new Type* [temp_size];

        for(int i=0; i<size; ++i) { //copy old arr to the new one
            temp_arr[i] = arr[i];
        }

        delete [] arr;      //del old arr
        arr = temp_arr;     //update arr
        size = temp_size;   //update size
    }

    arr[tail++] = item;
    reheapUp(0, tail-1);
}

//pop first elemnt arr
template <class Type>
Type* Queue<Type>::pop() {
    if (isEmpty()) {
        cout << "pop gagal!" << endl;
        exit(1);
    }

    Type* rval = arr[0];
    arr[0] = arr[tail-1];
    --tail;
    reheapDown(0, tail-1);

    return rval;
}

template <class Type>
Type* Queue<Type>::head(){
    if (isEmpty()) {
        cout << "pop gagal!" << endl;
        exit(1);
    }

    return arr[0];
}

template <class Type>
bool Queue<Type>::isEmpty() const {
    return (tail <= 0);
}

template <class Type>
bool Queue<Type>::isFull() const {
    return (tail >= size);
}

//fix heap upward
template <class Type>
void Queue<Type>::reheapUp(int root, int bottom) {
    int parent;     //parent of the bottom

    if (bottom > root) {
        parent = (bottom - 1) / D;

        if (*arr[parent] > *arr[bottom]) {
            swap(arr[parent], arr[bottom]);
            reheapUp(root, parent);
        }
    }
}

//fix heap downward
template <class Type>
void Queue<Type>::reheapDown(int root, int bottom) {
    int minChild, firstChild, child;

    firstChild = root * D + 1;

    if (firstChild <= bottom) {
        minChild = firstChild;

        for (int i=2; i<= D; ++i){
            child = root * D + i;
            if (child <= bottom) {
                if (*arr[child] < *arr[minChild]){
                    minChild = child;
                }
            }
        }

        if (*arr[root] > *arr[minChild]) {
            swap(arr[root], arr[minChild]);
            reheapDown(minChild, bottom);
        }

    }

}

template <class Type>
void Queue<Type>::swap(Type* &a, Type* &b) {
    Type* c;

    c = a;
    a = b;
    b = c;
}


#endif // PRIOQUEUE_H
