//
// Created by kuba on 23.03.2025.
//

#ifndef AIZO_MAIN_H
#define AIZO_MAIN_H
#include <iostream>
#include "Structure.h"
#include "Timer.h"


class Main {
public:
    void start(int arg_number, char* arg_values[]);
private:
    void file_test_mode(int algorithm, int type, std::string& input, std::string& output, Timer timer, int modifier);
    void benchmark_mode(int algorithm, int type, int amount, int distribution, std::string& output, Timer timer, int modifier);
    void static help_mode();
    template <typename T>
    int sorting(int algorithm, Structure<T>& structure, Timer timer, int modifier);
    template <typename T>
    void process_file(int algorithm, std::string input, std::string output, Timer timer, int modifier);
    template <typename T>
    void process_test(int algorithm, int type, int amount, int distribution, std::string& output, Timer timer, int modifier);
};


#endif //AIZO_MAIN_H
