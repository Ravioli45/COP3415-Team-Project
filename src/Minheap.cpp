#include "Minheap.h"

// default constructor for MinHeap
//
// creates a MinHeap with a capacity of 4
template <typename T>
MinHeap<T>::MinHeap() : MinHeap(4){};

// creates a MinHeap of a given capacity
template <typename T>
MinHeap<T>::MinHeap(unsigned the_capacity){
    size = 0; 
    capacity = the_capacity;
    array = new T[capacity];
}
// copy constructor fro MinHeap
template <typename T>
MinHeap<T>::MinHeap(const MinHeap& other){
    size = other.size;
    capacity = other.capacity;
    array = new T[capacity];

    for(unsigned i = 1; i <= size; i++){
        array[i] = other.array[i];
    }
}
// copy assignment for MinHeap
template<typename T>
MinHeap<T>& MinHeap<T>::operator=(MinHeap other){
    swap(*this, other);
    return *this;
}
// destructor for MinHeap
template<typename T>
MinHeap<T>::~MinHeap(){
    delete [] array;
}

// pushes a new element to the minheap
template<typename T>
void MinHeap<T>::push(const T& value){

    // double capacity of minheap if new element won't fit on it
    if(size+1 == capacity){
        resize(2*capacity);
    }

    // increase size, add element at end of array, percolate up the new_element
    size++;
    array[size] = value;
    percolate_up(size);
}

// removes and returns the smallest element in the minheap
template<typename T>
T MinHeap<T>::pop(){
    //remove top element, move bottom element to top, percolate down top element
    T result = array[1];

    array[1] = array[size];
    size--;

    percolate_down(1);

    return result;
}

template<typename T>
void MinHeap<T>::percolate_up(unsigned index){
    // moves element up the minheap if it is smaller than its parent
    if(index > 1){
        if(array[index/2] > array[index]){
            utils::swap(array[index/2], array[index]);
            percolate_up(index/2);
        }
    }
}

template<typename T>
void MinHeap<T>::percolate_down(unsigned index){
    int min_index = -1;

    // find index of child with smaller value
    if(2*index <= size){
        min_index = 2*index;
        if(2*index + 1 <= size){
            min_index = array[2*index+1] < array[2*index] ? 2*index+1 : min_index;
        }
    }

    // min_index == 1 when index has no children
    if(min_index != -1){
        // if value is greater than smallest child then swap
        // value with child and percolate down on child index
        if(array[index] > array[min_index]){
            utils::swap(array[index], array[min_index]);
            percolate_down(min_index);
        }
    }
}

// used internally within minheap to increase the capacity of the minheap
template<typename T>
void MinHeap<T>::resize(unsigned new_capacity){
    T* new_array = new T[new_capacity];

    for(unsigned i = 1; i <= size; i++){
        new_array[i] = array[i];
    }
    capacity = new_capacity;

    delete [] array;
    array = new_array;
}

// returns size of the minheap
template<typename T>
unsigned MinHeap<T>::get_size() const{
    return size;
}

// returns capacity of the minheap
template<typename T>
unsigned MinHeap<T>::get_capacity() const{
    return capacity;
}

// returns if the minheap is empty
template<typename T>
bool MinHeap<T>::is_empty() const{
    return size < 1;
}

// swaps two minheaps, used by copy assignment
template<typename T>
void swap(MinHeap<T>& first, MinHeap<T>& second){
    utils::swap(first.size, second.size);
    utils::swap(first.capacity, second.capacity);
    utils::swap(first.array, second.array);
}