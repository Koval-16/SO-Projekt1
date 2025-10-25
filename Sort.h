//
// Created by kuba on 05.04.2025.
//

#ifndef AIZO_SORT_H
#define AIZO_SORT_H
#include "Structure.h"

class Sort {
public:
    template<typename T>
    void insertion_sort(Structure<T>& structure); // InsertionSort
    template<typename T>
    void heap_sort(Structure<T>& structure); // HeapSort
    template<typename T>
    void shell_sort(Structure<T>& structure, int modifier); // ShellSort
    template<typename T>
    void quick_sort(Structure<T>& structure, int left, int right, int modifier); // QuickSort
    template<typename T>
    void drunk_sort(Structure<T>& structure); // Drunk HeapSort
    template <typename T>
    void quick_sort_multithreaded(Structure<T> &structure, int left, int right, int modifier);
    template <typename T>
    void multi_quick_sort(Structure<T> &structure, int left, int right, int modifier);
    template<typename T>
    bool verify(Structure<T>& structure); // Function to verify
private:
    template<typename T>
    int partition(Structure<T>& structure, int left, int right, int modifier); // Partition (QS)
    template<typename T>
    bool compare(T first, T second);
    template<typename T>
    void heapify(Structure<T>& structure, int index, int heapsize); // Heapify (HS)
    template<typename T>
    int build_heap(Structure<T>& structure, int start); // Build Heap (HS)
    int space(int size, int modifier); // Space (SS)
};


#endif //AIZO_SORT_H
