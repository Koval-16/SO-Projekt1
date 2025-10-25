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
            sort.heap_sort(structure);
            break;
        case 2:
            sort.heap_sort(structure);
            for(int i=0; i<size/2; i++){
                T temp = structure.get_element(i);
                structure.get_array()[i] = structure.get_element(size-i-1);
                structure.get_array()[size-i-1] = temp;
            }
            break;
        case 3:
            sort.heap_sort(structure);
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
            sort.heap_sort(str_help);
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

template<>
BoardGame RandomHandler::choose_random<BoardGame>() {
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int> dist1(1,10);
    std::uniform_int_distribution<int> dist2(5,480);
    std::uniform_int_distribution<int> dist3(5,25);
    std::uniform_int_distribution<int> dist4(0,25);
    std::string characters = "abcdefghijklmnopqrstuwvxyz";
    int length = dist3(rng);
    std::string name;
    for(int j=0; j<length; j++) name+= characters[dist4(rng)];
    int players = dist1(rng);
    int time = dist2(rng);
    int difficulty = dist1(rng);
    int happiness = dist1(rng);
    BoardGame game(name,players,time,difficulty,happiness);
    return game;
}

template void RandomHandler::create_array<int>(int amount, Structure<int> &structure, int distribution);
template void RandomHandler::create_array<float>(int amount, Structure<float> &structure, int distribution);
template void RandomHandler::create_array<char*>(int amount, Structure<char*> &structure, int distribution);
template void RandomHandler::create_array<BoardGame>(int amount, Structure<BoardGame> &structure, int distribution);
