cmake_minimum_required(VERSION 3.23)
project(CheckersGame_Clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(CheckersGame_Clion main.cpp CheckersGame.cpp CheckersGame.h consts.h GameBoard.cpp GameBoard.h staticFunctions.cpp staticFunctions.h)
