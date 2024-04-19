#include "Hash.h"
#include <iostream>

template<>
inline unsigned long hash(const std::string& str){
    //return std::hash<std::string>{}(str);
    //unsigned long hash = 5381;
    unsigned long hash = 0;
    for(char c : str){
        //hash = ((hash << 5) + hash) + c;
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    
    return hash;
}

template<>
inline unsigned long hash(const int& n){
    return n;
}

template<class K, class V>
HashMap<K, V>::HashMap() : HashMap(16){}

template<class K, class V>
HashMap<K, V>::HashMap(unsigned the_capacity){
    size = 0;
    capacity = the_capacity;
    array = new HashEntry<K, V>*[capacity];
    for(unsigned i = 0; i < capacity; i++){
        array[i] = nullptr;
    }
}

template<class K, class V>
HashMap<K, V>::~HashMap(){
    int counter = 0;
    for(unsigned i = 0; i < capacity; i++){
        HashEntry<K, V>* current = array[i];

        while(current != nullptr){
            HashEntry<K, V>* next = current->next;
            //std::cout << "deleting: " << current->key << ':' << current->value << std::endl;
            delete current;
            current = next;
            counter++;
        }
    }
    //std::cout << "deleted: " << counter << std::endl;
    delete [] array;
}

template<class K, class V>
HashMap<K, V>::HashMap(const HashMap& other){
    this->capacity = other.capacity;
    this->size = 0;

    this->array = new HashEntry<K, V>*[this->capacity];

    for(unsigned i = 0; i < other.capacity; i++){
        HashEntry<K, V>* current = other.array[i];
        while(current != nullptr){
            this->put(current->key, current->value);
            current = current->next;
        }
    }
}

template<class K, class V>
HashMap<K, V>& HashMap<K, V>::operator=(HashMap other){
    swap(*this, other);
    return *this;
}

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

template<class K, class V>
void HashMap<K, V>::rehash(unsigned new_capacity){
    //std::cout << "rehash: ";
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

template<class K, class V>
void swap(HashMap<K, V>& first, HashMap<K, V>& second){
    utils::swap(first.size, second.size);
    utils::swap(first.capacity, second.capacity);
    utils::swap(first.array, second.array);
}

template<class K, class V>
HashEntry<K, V>::HashEntry(const K& the_key, const V& the_value){
    key = the_key;
    value = the_value;

    next = nullptr;
}
