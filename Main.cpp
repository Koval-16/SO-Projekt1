//
// Created by kuba on 23.03.2025.
//

#include "Main.h"
#include <iostream>
#include "Structure.h"
#include "FileHandler.h"
#include "RandomHandler.h"
#include "Sort.h"
#include "Timer.h"

// Main function
int main(int arg_number, char* arg_values[]){
    srand(time(nullptr));
    Main program;
    program.start(arg_number, arg_values);
    return 0;
}

// Start function, it accepts arguments and run chosen mode.
void Main::start(int arg_number, char *arg_values[]) {
    if(arg_number<2){
        help_mode();
        return;
    }
    Timer timer;
    std::string mode = arg_values[1];
    if(mode=="--file"){
        try{
            int algorithm = std::stoi(arg_values[2]);
            int type = std::stoi(arg_values[3]);
            std::string input = arg_values[4];
            std::string output = arg_values[5];
            int modifier = (arg_number>=7) ? std::stoi(arg_values[6]) : 0;
            if(algorithm<0 || algorithm>5) throw std::exception();
            if(type<0 || type>3) throw std::exception();
            file_test_mode(algorithm, type, input, output, timer, modifier);
        }
        catch (...){
            help_mode();
        }
    }
    else if(mode=="--test"){
        try{
            int algorithm = std::stoi(arg_values[2]); // InsertSort, HeapSort, ShellSort, QuickSort
            int type = std::stoi(arg_values[3]); // int, float, charstring, boardgames
            int amount = std::stoi(arg_values[4]);
            int distribution = std::stoi(arg_values[5]);
            std::string output = arg_values[6];
            int modifier = (arg_number>=8) ? std::stoi(arg_values[7]) : 0;
            if(algorithm<0 || algorithm>5) throw std::exception();
            if(type<0 || type>3) throw std::exception();
            if(amount<0) throw std::exception();
            benchmark_mode(algorithm, type, amount, distribution ,output, timer, modifier);
        }
        catch(...){
            help_mode();
        }
    }
    else help_mode();
}

// Single file test mode, runs sorting algorithm on array from .txt file
void Main::file_test_mode(int algorithm, int type, std::string& input, std::string& output, Timer timer, int modifier) {
    std::cout << "File Test Mode" << std::endl;
    switch(type){
        case 0:
            process_file<int>(algorithm,input,output,timer,modifier);
            break;
        case 1:
            process_file<float>(algorithm,input,output,timer,modifier);
            break;
        case 2:
            process_file<char*>(algorithm,input,output,timer,modifier);
            break;
        default:
            break;
    }
}

// Process the algorithm with chosen parameters on Structure from .txt file.
template <typename T>
void Main::process_file(int algorithm, std::string input, std::string output, Timer timer, int modifier){
    Structure<T> structure;
    FileHandler::read_file(input, structure);
    std::cout << sorting(algorithm,structure, timer, modifier) << std::endl;
    FileHandler::save_to_file(output,structure);
}

// Process the algorithm 100 times, with chosen parameters on randomly drawn Structure.
template <typename T>
void Main::process_test(int algorithm, int type, int amount, int distribution, std::string& output, Timer timer, int modifier) {
    for(int i=1; i<=10; i++){
        Structure<T> structure;
        RandomHandler::create_array(amount,structure,distribution);
        int sorting_time = sorting(algorithm,structure,timer, modifier);
        FileHandler::save_test(i, algorithm, type, amount, distribution, sorting_time, output, modifier);
    }
}

// Benchmark mode, runs multiple tests.
void Main::benchmark_mode(int algorithm, int type, int amount, int distribution, std::string& output, Timer timer, int modifier) {
    std::cout << "Benchmark Mode" << std::endl;
    switch(type){
        case 0:
            process_test<int>(algorithm,type,amount,distribution,output,timer,modifier);
            break;
        case 1:
            process_test<float>(algorithm,type,amount,distribution,output,timer,modifier);
            break;
        case 2:
            process_test<char*>(algorithm,type,amount,distribution,output,timer,modifier);
            break;
        default:
            break;
    }
}

// Displays help message.
void Main::help_mode() {
    std::cout << "Help Mode\n"
                 "FILE TEST MODE:\n"
                 "\tUsage:\n"
                 "\t./aizo --file <algorithm> <type> <inputFile> <outputFile>\n"
                 "\t<algorithm>\tSorting algorithm to use: 0-Normal Quick Sort, 1-Multithreaded Quick Sort\n"
                 "\t<type>\t\tData type to use: 0-int, 1-float, 2-char[]\n"
                 "\t<inputFile>\tName of input file storing data, with extension\n"
                 "\t<outputFile>\tName of output file, where sorted data will be saved.\n"
                 "\t[modifier]\tUsed for:\n"
                 "\t\tQuicksort: Pivot(0-middle, 1-left, 2-right, 3-random)\n"
                 "\t\tif not provided: modifier=0.\n"
                 "\nBENCHMARK MODE:\n\tUsage:\n"
                 "\t./aizo --test <algorithm> <type> <amount> <distribution> <outputFile>\n"
                 "\t<algorithm>\tSorting algorithm to use: 0-Normal Quick Sort, 1-Multithreaded Quick Sort\n"
                 "\t<type>\t\tData type to use: 0-int, 1-float, 2-char[]\n"
                 "\t<amount>\tAmount of elements to be generated\n"
                 "\t<distribution>\tDistribution of elements: 0-random, 1-sorted, 2-sorted oppositely, 3-first half sorted, 4-second half sorted\n"
                 "\t<outputFile>\tName of output file, where collected information will be saved.\n"
                 "\t[modifier]\tUsed for:\n"
                 "\t\tQuicksort: Pivot(0-middle, 1-left, 2-right, 3-random)\n"
                 "\t\tif not provided: modifier=0.\n"
                 "\nHELP MODE:\n\tUsage:\n"
                 "\t./aizo --help\n"<< std::endl;
}

// Measures execution time of an algorithm.
template <typename T>
int Main::sorting(int algorithm, Structure<T>& structure, Timer timer, int modifier) {
    Sort sort;
    timer.reset();
    timer.start();
    switch(algorithm){
        case 0:
            sort.quick_sort(structure,0,structure.get_size()-1, modifier);
            break;
        case 1:
            sort.quick_sort_multithreaded(structure,0,structure.get_size()-1, modifier);
            break;
        default:
            break;
    }
    timer.stop();
    if(!sort.verify(structure)) std::cout << "sth went wrong during sorting" << std::endl;
    return timer.result();
}