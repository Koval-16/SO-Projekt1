//
// Created by kuba on 23.03.2025.
//

#ifndef AIZO_STRUCTURE_H
#define AIZO_STRUCTURE_H
#include <iostream>

// Class Structure stores an array of elements, which will be sorted.
template <typename T>
class Structure {
private:

public:
    T *array;
    int size;
    int capacity;
    //Structure(int amount);
    Structure();
    ~Structure();
    void add_element(T element);
    void set_capacity(int value);
    T get_element(int index);
    int get_size();
    T* get_array();
};


#endif //AIZO_STRUCTURE_H
