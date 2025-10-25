//
// Created by kuba on 23.03.2025.
//

#include "Structure.h"
#include "BoardGame.h"
#include <cstring>

template<typename T>
Structure<T>::Structure(){
    size = 0;
    capacity = 1;
    array = new T[capacity];
}

template<typename T>
Structure<T>::~Structure() {
    if constexpr (std::is_same<T, char*>::value) for (int i = 0; i < size; i++) delete[] array[i];
    delete[] array;
}

// Adds an element to the array.
template<typename T>
void Structure<T>::add_element(T element) {
    if(size==capacity){
        capacity++;
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++) {
            if constexpr (std::is_same<T, char*>::value) {
                int len = std::strlen(array[i]);
                temp[i] = new char[len + 1];
                std::strcpy(temp[i], array[i]);
                delete[] array[i];
            }
            else temp[i] = array[i];

        }
        delete[] array;
        array = temp;
    }
    if constexpr (std::is_same<T, char*>::value) {
        int len = std::strlen(element);
        array[size] = new char[len + 1];
        std::strcpy(array[size], element);
    }
    else array[size] = element;
    size++;
}

// Sets capacity of the array to the specified value.
template<typename T>
void Structure<T>::set_capacity(int value) {
    capacity = value;
    T* temp = new T[capacity];
    for (int i = 0; i < size; i++) {
        if constexpr (std::is_same<T, char*>::value) {
            int len = std::strlen(array[i]);
            temp[i] = new char[len + 1];
            std::strcpy(temp[i], array[i]);
            delete[] array[i];
        }
        else temp[i] = array[i];
    }
    delete[] array;
    array = temp;
}

template<typename T>
T Structure<T>::get_element(int index) {
    return array[index];
}

template<typename T>
int Structure<T>::get_size() {
    return size;
}

template<typename T>
T* Structure<T>::get_array() {
    return array;
}

template class Structure<int>;
template class Structure<float>;
template class Structure<char*>;
template class Structure<BoardGame>;