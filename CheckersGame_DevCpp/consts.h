//
// Created by phoenix on 1/19/2023.
//

#ifndef CHECKERSGAME_CLION_CONSTS_H
#define CHECKERSGAME_CLION_CONSTS_H

#include <string>

const std::string RESET_CM_C = "reset";
const std::string REDO_CM_C = "redo";
const std::string EXIT_CM_C = "exit";
const std::string REPORT_CM_C = "report";

const int DEFAULT_COLOR = 7;
const int WHITE_SIDE_COLOR = 10;
const int BLACK_SIDE_COLOR = 4;
const int DIVIDER_COLOR = 1;
const int SHOW_INFO_COLOR = 6;
const int REQ_INFO_COLOR = 3;
const int GROUND_COLOR = 5;
const int NOTICE_COLOR = 9;


const char WHITE_C = 'x';//White character when printing play ground;
const char WHITE_KING_C = 'X';//White king character when printing play ground;
const char BLACK_C = 'o';//Black character when printing play ground;
const char BLACK_KING_C = 'O';//Black king character when printing play ground;
const char EMPTY_C = ' ';//Don't Change this please, or your ground print will be ugly;

const char POS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','i',
                    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q','r',
                    's', 't', 'u', 'v', 'w', 'x', 'y','z'};

struct Coordinates{
    int x;
    int y;
};

enum Piece {WHITE, WHITE_KING, BLACK, BLACK_KING, EMPTY, NON};
enum Side {A_WHITE, B_BLACK, NONE};

#endif //CHECKERSGAME_CLION_CONSTS_H
