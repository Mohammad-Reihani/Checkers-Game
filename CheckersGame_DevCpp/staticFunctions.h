//
// Created by phoenix on 1/19/2023.
//

#ifndef CHECKERSGAME_CLION_STATICFUNCTIONS_H
#define CHECKERSGAME_CLION_STATICFUNCTIONS_H

#include <iostream>
#include <vector>
#include <windows.h>
#include "consts.h"
#include "CheckersGame.h"

using namespace std;

class staticFunctions {
    static string extractNextElementFromLine(string &line);
    static bool isCommand(string basicString);
    static string doCommand(CheckersGame &checkersGame, const string& cm);

public:
    static void sayHello();

    static int getPlayGroundSize();

    static Side getStarterSide();

    static string getLineAndExtractCoordinatesOrCommand(CheckersGame &checkersGame,string line, Coordinates cds[]);

    static void handleDivider();

    static int digitsCount(int input);

    static Coordinates stringToCoordinates(string cString);

    static string getMoveErrorByCode(int error_code);

    static string sideToString(Side side);

    static bool isForward(Side side, Coordinates from, Coordinates to);

    static bool checkCoordinatedDistance(Coordinates from, Coordinates to, int limit);

    static string CoordinatesToConsoleInput(Coordinates cd);

    static void printTos(vector<Coordinates> d);
};


#endif //CHECKERSGAME_CLION_STATICFUNCTIONS_H
