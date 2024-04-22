#ifndef UTILS_H
#define UTILS_H
#include <vector>

namespace utils{
    template<typename T>
    void swap(T& a, T& b);

    template<typename T>
    void sort(std::vector<T>& vec);
}

#endif
