//
// Created by kuba on 28.03.2025.
//

#ifndef AIZO_RANDOMHANDLER_H
#define AIZO_RANDOMHANDLER_H
#include "Structure.h"
#include "BoardGame.h"


class RandomHandler {
public:
    template <typename T>
    static void create_array(int amount, Structure<T>&structure, int distribution);
private:
    template<typename T>
    static T choose_random();
};


#endif //AIZO_RANDOMHANDLER_H
