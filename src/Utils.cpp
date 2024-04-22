#ifndef UTILS_CPP
#define UTILS_CPP

#include "Utils.h"

namespace utils{
    template<typename T>
    void swap(T& a, T& b){
        T temp = a;
        a = b;
        b = temp;
    }

    template<typename T>
    void sort(std::vector<T>& vec){
        for(long unsigned i = 0; i < vec.size(); i++){

            unsigned long min_index = i;

            for(long unsigned j = i+1; j < vec.size(); j++){
                if(vec[j] < vec[min_index]){
                    min_index = j;
                }
            }
            if(min_index != i){
                swap(vec[min_index], vec[i]);
            }
        }
    }
}

#endif
