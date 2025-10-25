//
// Created by kuba on 28.03.2025.
//

#include "BoardGame.h"
#include <sstream>
#include <math.h>

// Constructor, it requires:
// name = name of the game
// players = amount of players
// time = how long the game is
// difficulty = how complicated the game is
// happiness = how much fun it gives
// ratio = coefficient, which will be used as a key in sorting - calculated by a formula
BoardGame::BoardGame(std::string name, int players, int time, int difficulty, int happinness) {
    this->name = name;
    this->players = players;
    this->time = time;
    this->difficulty = difficulty;
    this->happinness = happinness;
    ratio = (pow(happinness,1.25) * players)/(difficulty*log(time+10));
}

BoardGame::BoardGame() = default;

double BoardGame::get_ratio() {
    return ratio;
}

std::string BoardGame::to_string() {
    std::ostringstream oss;
    oss << name << " " << players << " " << time << " " << difficulty << " " << happinness;
    return oss.str();
}
