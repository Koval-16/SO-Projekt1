//
// Created by kuba on 28.03.2025.
//

#include "RandomHandler.h"
#include <random>
#include "Sort.h"

template <typename T>
void RandomHandler::create_array(int amount, Structure<T> &structure, int distribution) {
    int size;
    if(distribution>=0 && distribution<=2) size=amount;
    else size=amount/2;
    structure.set_capacity(size);
    for(int i=0; i<size; i++){
        T element = choose_random<T>();
        structure.add_element(element);
    }
    Sort sort;
    switch (distribution) {
        case 0:
            break;
        case 1:
            sort.quick_sort(structure,0,structure.get_size()-1,0);
            break;
        case 2:
            sort.quick_sort(structure,0,structure.get_size()-1,0);
            for(int i=0; i<size/2; i++){
                T temp = structure.get_element(i);
                structure.get_array()[i] = structure.get_element(size-i-1);
                structure.get_array()[size-i-1] = temp;
            }
            break;
        case 3:
            sort.quick_sort(structure,0,structure.get_size()-1,0);
            for(int i=size; i<amount; i++){
                T element = choose_random<T>();
                structure.add_element(element);
            }
            break;
        case 4:
            Structure<T> str_help;
            for(int i=0; i<size; i++){
                T element = choose_random<T>();
                str_help.add_element(element);
            }
            sort.quick_sort(structure,0,structure.get_size()-1,0);
            for(int i=0; i<size; i++){
                structure.add_element(str_help.get_array()[i]);
            }
            break;
    }
}

template<>
int RandomHandler::choose_random<int>() {
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    return dist(rng);
}

template<>
float RandomHandler::choose_random<float>() {
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_real_distribution<float> dist(-1e38f, 1e38f);
    return dist(rng);
}

template<>
char* RandomHandler::choose_random<char*>() {
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int> string_len(1,20);
    std::uniform_int_distribution<char> char_dist('a','z');
    int length = string_len(rng);
    char* random_string = new char[length + 1];
    for (int j = 0; j < length; j++) {
        random_string[j] = char_dist(rng);  // 'a' + [0-25] -> od 'a' do 'z'
    }
    random_string[length] = '\0';
    return random_string;
}



template void RandomHandler::create_array<int>(int amount, Structure<int> &structure, int distribution);
template void RandomHandler::create_array<float>(int amount, Structure<float> &structure, int distribution);
template void RandomHandler::create_array<char*>(int amount, Structure<char*> &structure, int distribution);