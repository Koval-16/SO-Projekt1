//
// Created by kuba on 28.03.2025.
//

#ifndef AIZO_BOARDGAME_H
#define AIZO_BOARDGAME_H
#include <iostream>

// Class representing board game, it will be used as one of datatypes in sorting algorithms.
class BoardGame {
public:
    BoardGame(std::string name, int players, int time, int difficulty, int happinness);
    BoardGame();
    double get_ratio();
    std::string to_string();
private:
    std::string name;
    int players{};
    int time{};
    int difficulty{};
    int happinness{};
    double ratio{};
};


#endif //AIZO_BOARDGAME_H
