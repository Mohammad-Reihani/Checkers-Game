//
// Created by phoenix on 1/19/2023.
//

#ifndef CHECKERSGAME_CLION_CHECKERSGAME_H
#define CHECKERSGAME_CLION_CHECKERSGAME_H

#include "GameBoard.h"

class CheckersGame {
private:
    struct DoubleCoordinates{
        Coordinates from;
        Coordinates to;
    };

    struct DeletedOnTurn{
        Coordinates deleted;
        Piece deletedP;
    };

    struct KingMoves{
        Coordinates cd;
        int moves;
    };

    int countBlackMoves = 0;
    int countWhiteMoves = 0;
    int countTotalMoves = 0;
    Side currentSide = A_WHITE;
    GameBoard playGround;
    vector<Coordinates> movablePieces_white;
    vector<Coordinates> movablePieces_black;
    vector<Coordinates> PiecesThatCanHit_white;
    vector<Coordinates> PiecesThatCanHit_black;

    vector<DoubleCoordinates> recordedMoves;
    vector<Side> recordedSides;
    vector<DeletedOnTurn> recordedDeletedPieces;
    vector<KingMoves> recordedKingsMoves;

    //this function updates both black and white side at the same time;
    void updateInHittingPositionPieces();

    //only updates the given side movable pieces;
    void updateMovablePieces(Side side);

public:
    CheckersGame();
    explicit CheckersGame(int gameBoardSize);


    void playRound(Coordinates from, Coordinates to);//the heart of this class

    void resetTheGame(Side starterSide);

    void printGround();

    void printAvailablePiecesToMove();

    void printAvailableMoves();

    //*****************************************************************************

    bool sideDoesNotMatch(Side side, Coordinates coordinates);

    bool checkCrossJumpAndEmpty(Coordinates from, Coordinates to);

    //*****************************************************************************

    bool kingCanHit(Coordinates cd, Side oppositeSide);

    bool pieceCanHit(Coordinates cd, Side oppositeSide);

    bool canHit(Coordinates cd, Side side);

    bool isMovable(Coordinates cd, Side side);

    void checkAndSetKings();

    bool checkWinner(string &result);

    bool isNotCoordinates(const string& cString);

    //*****************************************************************************


    void clearHM();

    void printTurnGuider();

    //*****************************************************************************

    int getBlackPiecesCounts();

    int getWhitePiecesCounts();

    Side getCurrentSide();

    void setStarterSide(Side side);

    string redoIfPossible();

    string giveReport();

    int getWhiteRemovedPiecesCounts();

    int getBlackRemovedPiecesCounts();

    void addKingMovesAndUpdateCoordinates(Coordinates cd, Coordinates to);

    int findAndGetKingMoves(Coordinates cd);

    vector<Coordinates> getDestinationsForHitter(const Coordinates &cd);

    vector<Coordinates> getDestinationsForNormalMovers(const Coordinates &cd);

    void removeKingFromRecorded(Coordinates cd);

    void removeOneMoveFromKingAndUpdateCd(Coordinates cd, Coordinates from);
};



#endif //CHECKERSGAME_CLION_CHECKERSGAME_H
