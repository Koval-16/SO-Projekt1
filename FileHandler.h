//
// Created by kuba on 23.03.2025.
//

#ifndef AIZO_FILEHANDLER_H
#define AIZO_FILEHANDLER_H

#include <iostream>
#include "Structure.h"
#include "BoardGame.h"

// Class FileHandler handles files operations such as reading and saving
// for both: single file mode and benchmark mode.
class FileHandler {

public:
    template<typename T>
    static void read_file(std::string& filename, Structure<T>& structure);
    // These functions save results to files in single file mode
    template<typename T>
    static void save_to_file(std::string& filename, Structure<T>& structure);
    static void save_test(int i, int algorithm, int type, int amount, int distribution, int sorting_time, std::string& output, int modifier);
private:
    template<typename T>
    static T convert(std::string& text); // Converts String from .txt file to other datatype
    template<typename T>
    static std::string write_element(T element); // Writes an element in .txt file
};

#endif //AIZO_FILEHANDLER_H
