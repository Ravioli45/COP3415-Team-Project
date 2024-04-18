#ifndef HASH_H
#define HASH_H

#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
inline unsigned long hash(const T& item);

template<>
inline unsigned long hash(const std::string& str);

template<>
inline unsigned long hash(const int& n);

template<class K, class V>
class HashEntry;

template<class K, class V>
class HashMap;

template<class K, class V>
void swap(HashMap<K, V>& first, HashMap<K, V>& second);

template<class K, class V>
class HashMap{
    private:
        unsigned size;
        unsigned capacity;
        HashEntry<K, V>* *array;

        constexpr static double load_factor = 0.75;

        void rehash(unsigned new_capacity);
    public:
        HashMap();
        HashMap(unsigned the_capacity);
        HashMap(const HashMap& other);
        HashMap& operator=(HashMap other);
        V& operator[](const K& key);
        ~HashMap();

        void put(const K& key, const V& value);
        V& get(const K& key) const;

        friend void swap<>(HashMap& first, HashMap& second);
};

template<class K, class V>
class HashEntry{
    private:
        K key;
        V value;
        HashEntry* next;

        HashEntry(const K& the_key, const V& the_value);

    friend class HashMap<K, V>;
};


template class HashMap<std::string, int>;
template class HashMap<std::string, std::vector<std::string>>;
template class HashMap<std::string, bool>;

#endif