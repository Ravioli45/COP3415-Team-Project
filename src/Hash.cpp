#include "Hash.h"
#include <iostream>

// uses sdbm hash to hash strings
//
// source: http://www.cse.yorku.ca/~oz/hash.html
template<>
inline unsigned long hash(const std::string& str){
    unsigned long hash = 0;
    for(char c : str){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    
    return hash;
}

// hash funtion for ints
template<>
inline unsigned long hash(const int& n){
    return n;
}

// default constructor for hashmap, creates hashmap with capacity of 16
template<class K, class V>
HashMap<K, V>::HashMap() : HashMap(16){}

// creates a hashmap with a given capacity
template<class K, class V>
HashMap<K, V>::HashMap(unsigned the_capacity){
    size = 0;
    capacity = the_capacity;
    array = new HashEntry<K, V>*[capacity];
    for(unsigned i = 0; i < capacity; i++){
        array[i] = nullptr;
    }
}

// destructor for hashmap
template<class K, class V>
HashMap<K, V>::~HashMap(){
    int counter = 0;
    for(unsigned i = 0; i < capacity; i++){
        HashEntry<K, V>* current = array[i];

        while(current != nullptr){
            HashEntry<K, V>* next = current->next;
            // delete all HashEntry in the hashmap
            delete current;
            current = next;
            counter++;
        }
    }
    // delete array of HashEntry*
    delete [] array;
}

// copy constructor for HashMap
template<class K, class V>
HashMap<K, V>::HashMap(const HashMap& other){
    this->capacity = other.capacity;
    this->size = 0;

    this->array = new HashEntry<K, V>*[this->capacity];

    // put all elements from other HashMap into this HashMap
    for(unsigned i = 0; i < other.capacity; i++){
        HashEntry<K, V>* current = other.array[i];
        while(current != nullptr){
            this->put(current->key, current->value);
            current = current->next;
        }
    }
}

// move constructor for hashmap, I needed this to make std::vector<HashMap> work
template<class K, class V>
HashMap<K, V>::HashMap(HashMap&& other) noexcept {
    this->size = other.size;
    this->capacity = other.capacity;
    this->array = other.array;
    other.size = 0;
    other.capacity = 0;
    other.array = nullptr;
}

// copy assignment for HashMap
template<class K, class V>
HashMap<K, V>& HashMap<K, V>::operator=(HashMap other){
    swap(*this, other);
    return *this;
}

// define [] operator for hashmap
//
// if the key being used does not exist then it is filled with the
// value returned by the default constructor for V
template<class K, class V>
V& HashMap<K, V>::operator[](const K& key){

    // rehashes early anticipating new element
    if((size+1)/capacity > HashMap::load_factor){
        rehash(2*capacity);
    }
    
    int index = hash<K>(key) % capacity;

    HashEntry<K, V>* previous = nullptr;
    HashEntry<K, V>* current = array[index];
    
    // return value if present in hashmap
    while(current != nullptr){
        if(current->key == key){
            return current->value;
        }
        previous = current;
        current = current->next;
    }
    
    //insert value if not present
    //std::cout << "insertion: " << V() << std::endl;
    HashEntry<K, V>* new_entry = new HashEntry<K, V>(key, V());
    size++;
    if(previous == nullptr){
        array[index] = new_entry;
        return array[index]->value;
    }
    else{
        previous->next = new_entry;
        return previous->next->value;
    }
}

// puts a key value pair into the HashMap
template<class K, class V>
void HashMap<K, V>::put(const K& key,const V& value){
    int index = hash<K>(key) % capacity;

    HashEntry<K, V>* previous = nullptr;
    HashEntry<K, V>* current = array[index];

    while(current != nullptr){
        if(current->key == key){
            current->value = value;
            return;
        }
        previous = current;
        current = current->next;
    }

    HashEntry<K, V>* new_entry = new HashEntry<K, V>(key, value);
    if(previous == nullptr){
        array[index] = new_entry;
    }
    else{
        previous->next = new_entry;
    }

    size++;
    if((double)size / capacity > HashMap::load_factor){
        rehash(2*capacity);
    }
}

// get the value associated with the given key
//
// throws if the given key is not already present in the HashMap
template<class K, class V>
V& HashMap<K, V>::get(const K &key) const{
    int index = hash<K>(key) % capacity;

    HashEntry<K, V>* current = array[index];
    
    while(current != nullptr){
        if(current->key == key){
            return current->value;
        }
        current = current->next;
    }

    throw std::out_of_range("key does not exist");
}

// returns true if a given key is present in the hashmap
// returns false otherwise
template<class K, class V>
bool HashMap<K, V>::has_key(const K& key) const{
    int index = hash<K>(key) % capacity;

    HashEntry<K, V>* current = array[index];

    while(current != nullptr){
        if(current->key == key){
            return true;
        }
        current = current->next;
    }

    return false;
}

// returns the keys of the hashmap in no particular order
template<class K, class V>
std::vector<K> HashMap<K, V>::keys() const{
    std::vector<K> result;

    for(unsigned i = 0; i < capacity; i++){
        HashEntry<K, V>* current = array[i];
        while(current != nullptr){
            result.push_back(current->key);
            current = current->next;
        }
    }

    return result;
}

// used to increase the capacity of the hashmap when necessary
//
// when increasing the capacity all elements must be rehashed
template<class K, class V>
void HashMap<K, V>::rehash(unsigned new_capacity){
    HashMap<K, V> that(new_capacity);

    for(unsigned i = 0; i < capacity; i++){
        HashEntry<K, V>* current = array[i];

        while(current != nullptr){
            that.put(current->key, current->value);

            current = current->next;
        }

    }

    swap(*this, that);

    //std::cout << capacity << std::endl;
}

// swaps two hashmaps with each other
template<class K, class V>
void swap(HashMap<K, V>& first, HashMap<K, V>& second){
    utils::swap(first.size, second.size);
    utils::swap(first.capacity, second.capacity);
    utils::swap(first.array, second.array);
}

// construcrs new hashmap with the given key and value
// and with no next hashentry
template<class K, class V>
HashEntry<K, V>::HashEntry(const K& the_key, const V& the_value){
    key = the_key;
    value = the_value;

    next = nullptr;
}

// makes a set with value in the hashDSU
template<typename T>
void HashDSU<T>::make_set(const T& value){
    parent[value] = value;
    rank[value] = 0;
}

// finds the representative of the set that contains the given value
// in the hashDSU
//
// uses path compression to maintain efficiency
template<typename T>
const T& HashDSU<T>::find_set(const T& value){
    if(!parent.has_key(value)){
        make_set(value);
    }

    if(value == parent[value])
        return value;
    return parent[value] = find_set(parent[value]);
}

// unions the sets containing the given elements
//
// uses union by rank to maintain efficinncy
template<typename T>
void HashDSU<T>::union_set(const T& first, const T& second){
    const T& rep1 = find_set(first);
    const T& rep2 = find_set(second);

    if(rep1 != rep2){
        if(rank[rep1] < rank[rep2]){
            parent[rep1] = rep2;
        }
        else if(rank[rep1] > rank[rep2]){
            parent[rep2] = rep1;
        }
        else{
            parent[rep2] = rep1;
            rank[rep1]++;
        }
    }
}
