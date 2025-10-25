cmake_minimum_required(VERSION 3.26)
project(aizo)

set(CMAKE_CXX_STANDARD 17)

add_executable(aizo
        main.cpp
        Main.cpp
        Main.h
        FileHandler.cpp
        FileHandler.h
        Structure.cpp
        Structure.h
        Timer.cpp
        Timer.h
        RandomHandler.cpp
        RandomHandler.h
        BoardGame.cpp
        BoardGame.h
        Sort.cpp
        Sort.h
)
