#ifndef MINHEAP_H
#define MINHEAP_H

#include "Utils.cpp"

template <typename T>
class MinHeap;

// swap function that gets used in the copy assignment of MinHeap
template<typename T>
void swap(MinHeap<T>& first, MinHeap<T>& second);

// template class for a MinHeap containing elements 
// of type T
template <typename T>
class MinHeap{
    private:
        unsigned size;
        unsigned capacity;
        // array is a heap allocated array holding the elements in the minheap
        T* array;

        void percolate_up(unsigned index);
        void percolate_down(unsigned index);
        void resize(unsigned new_capacity);
    public:
        MinHeap();
        MinHeap(unsigned the_capacity);
        MinHeap(const MinHeap& other);
        MinHeap& operator=(MinHeap other);
        ~MinHeap();

        void push(const T& value);
        T pop();

        unsigned get_size() const;
        unsigned get_capacity() const;

        bool is_empty() const;

    friend void swap<>(MinHeap& first, MinHeap& second);
};

#endif