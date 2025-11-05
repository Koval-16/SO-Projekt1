//
// Created by kuba on 05.04.2025.
//

#ifndef AIZO_SORT_H
#define AIZO_SORT_H
#include "Structure.h"

class Sort {
public:
    template<typename T>
    void quick_sort(Structure<T>& structure, int left, int right, int modifier); // QuickSort
    template <typename T>
    void quick_sort_multithreaded(Structure<T> &structure, int left, int right, int modifier);
    template<typename T>
    bool verify(Structure<T>& structure); // Function to verify
private:
    template<typename T>
    int partition(Structure<T>& structure, int left, int right, int modifier); // Partition (QS)
    template<typename T>
    bool compare(T first, T second);
};


#endif //AIZO_SORT_H
