//
// Created by kuba on 23.03.2025.
//

#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "Structure.h"
#include <cstring>

// Reads the file in single file mode, line by line, adding elements to Structure.
template<typename T>
void FileHandler::read_file(std::string &filename, Structure<T> &structure) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file.");
    std::string text;
    int index = 0;
    try{
        while(std::getline(file,text)){
            if(index==0) {
                int capacity = std::stoi(text);
                if(capacity<=0) throw std::runtime_error("Size of array incorrect");
                structure.set_capacity(capacity);
            }
            else{
                T value = convert<T>(text);
                structure.add_element(value);
            }
            index++;
        }
        if(structure.get_size()!=structure.capacity) throw std::exception();
    }
    catch (std::exception &e){
        throw std::runtime_error("Error occurred");
    }
}

// Converts String from .txt file to Integer number
template<>
int FileHandler::convert<int>(std::string &text) {
    return std::stoi(text);
}

// Converts String from .txt file to Float number
template<>
float FileHandler::convert<float>(std::string &text) {
    return std::stof(text);
}

// Converts String from .txt file to Array of Char
template<>
char* FileHandler::convert<char*>(std::string &text) {
    char* copy = new char[text.length() + 1];
    std::strcpy(copy, text.c_str());
    return copy;
}


// Save result of single test to a file (int, float, char[])
template<typename T>
void FileHandler::save_to_file(std::string& filename, Structure<T>& structure) {
    std::ofstream file(filename);
    file << structure.get_size() << std::endl;
    for(int i=0; i<structure.size; i++){
        std::string display = write_element<T>(structure.get_element(i));
        file << display << std::endl;
    }
    file.close();
}

template<>
std::string FileHandler::write_element<int>(int element) {
    return std::to_string(element);
}

template<>
std::string FileHandler::write_element<float>(float element) {
    return std::to_string(element);
}

template<>
std::string FileHandler::write_element<char*>(char* element) {
    return element;
}


// Saves result of a single test in benchmark mode. It saves info about all parameters used.
void FileHandler::save_test(int i, int algorithm, int type, int amount, int distribution, int sorting_time, std::string& output, int modifier) {
    std::ifstream file1(output);
    int count = 0;
    std::string line;
    while(std::getline(file1,line)) count++;
    file1.close();

    std::ofstream file2(output, std::ios::app);
    if(count>0) file2 << "\n";
    file2 << i << "\t";

    if(algorithm==0) file2 << "Quick" << "\t";
    else if(algorithm==1) file2 << "Multi" << "\t";

    if(type==0) file2 << "Intgr" << "\t";
    else if(type==1) file2 << "Float" << "\t";
    else if(type==2) file2 << "Char*" << "\t";

    file2 << amount << "\t";

    if(distribution==0) file2 << "Rndom" << "\t";
    else if(distribution==1) file2 << "Optim" << "\t";
    else if(distribution==2) file2 << "Pesim" << "\t";
    else if(distribution==3) file2 << "H1Sor" << "\t";
    else if(distribution==4) file2 << "H2Sor" << "\t";

    if(algorithm==2){
        if(modifier==0) file2 << "Frml1" << "\t";
        else if(modifier==1) file2 << "Frml2" << "\t";
    }
    else if(algorithm==0 || algorithm==1){
        if(modifier==0) file2 << "MID-P" << "\t";
        else if(modifier==1) file2 << "LFT-P" << "\t";
        else if(modifier==2) file2 << "RGH-P" << "\t";
        else if(modifier==3) file2 << "RAND-P" << "\t";
    }

    file2 << sorting_time;
}

template<typename T>
std::string FileHandler::write_element(T element) {
    throw std::exception();
}


template void FileHandler::save_to_file<int>(std::string&, Structure<int>&);
template void FileHandler::save_to_file<float>(std::string&, Structure<float>&);
template void FileHandler::save_to_file<char*>(std::string&, Structure<char*>&);

template void FileHandler::read_file(std::string &filename, Structure<int> &structure);
template void FileHandler::read_file(std::string &filename, Structure<float> &structure);
template void FileHandler::read_file(std::string &filename, Structure<char*> &structure);