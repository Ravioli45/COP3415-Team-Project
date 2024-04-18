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
}

#endif
