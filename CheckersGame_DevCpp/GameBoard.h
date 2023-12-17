//
// Created by phoenix on 1/19/2023.
//

#ifndef CHECKERSGAME_CLION_GAMEBOARD_H
#define CHECKERSGAME_CLION_GAMEBOARD_H


#include <iostream>
#include <vector>
#include <windows.h>
#include "consts.h"

using namespace std;

class GameBoard {
private:
    int gSize = 8;
    std::vector<std::vector<Piece>> ground;
    void printDivider() const;
public:
    void changeGH(Coordinates cd, Piece c);
    GameBoard() : ground(gSize, std::vector<Piece>(gSize, EMPTY)){
        resetPlayGround();
    }
    explicit GameBoard(int size) : ground(size, std::vector<Piece>(size, EMPTY)){
        this->gSize = size;
        resetPlayGround();
    }

    bool isKing(Coordinates cd);
    bool isEmpty(Coordinates cd);
    static bool isWhiteSpace(Coordinates cd);
    bool isPiece(Coordinates cd);

    int getWhiteCounts();
    int getBlackCounts();
    int getPlayGroundSize() const;//const methods are somehow... readOnly!!! yeah.(can not modify any non-static data member or call non-static member function of the class)
    Piece getPiece(Coordinates cd);
    Side getPieceSide(Coordinates cd);


    void resetPlayGround();
    void printGround();

    static char PieceToChar(Piece t);

//    string CoordinatesToConsoleInput(Coordinates cd);

    bool moveTo(Coordinates pieceP, Coordinates target);//returns if it was successful or not | the pieceP coordinates may be fake! & the target might not be empty;
    bool remove(Coordinates taw);//the taw coordinates may be fake!
    bool makeKing(Coordinates piece);//will change a piece to king level; | i'm not sure to put this here, any way: the piece might not be in a situation to be changed to a king;
    void undoKing(Coordinates cd);//if a piece is king, this function will un_king it
};



#endif //CHECKERSGAME_CLION_GAMEBOARD_H
