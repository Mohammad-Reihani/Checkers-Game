//
// Created by phoenix on 1/19/2023.
//

#include "GameBoard.h"

void GameBoard::printGround() {

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GROUND_COLOR);
    cout << "     ";
    for (int i = 0; i < gSize; i++) {
        cout << POS[i] << "   ";
    }
    cout << endl;
    printDivider();



    for (int i = gSize; i > 0 ; i--) {
        if(i > 9)
            cout << i << " ";
        else
            cout << i << "  ";
        for (int j = 0; j < gSize; ++j) {
            cout << "| ";
            if (getPieceSide(Coordinates{i-1, j}) == A_WHITE)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_SIDE_COLOR);
            else if (getPieceSide(Coordinates{i-1, j}) == B_BLACK)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_SIDE_COLOR);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GROUND_COLOR);

            cout << PieceToChar(getPiece(Coordinates{i-1, j}));

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GROUND_COLOR);

            cout << " ";
        }
        cout << "|" << endl;
        printDivider();

    }


    cout << "     ";
    for (int i = 0; i < gSize; i++) {
        cout << POS[i] << "   ";
    }
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}

void GameBoard::printDivider() const {
    cout << "   ";
    for (int j = 0; j < gSize; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

bool GameBoard::isEmpty(Coordinates cd) {
    if (cd.x < gSize && cd.x >= 0 && cd.y < gSize && cd.y >= 0)
        if (ground[cd.x][cd.y] == EMPTY)
            return true;
    return false;
}

bool GameBoard::moveTo(Coordinates pieceP, Coordinates target) {
    if (isEmpty(target) && !isEmpty(pieceP)){
        changeGH(target, getPiece(pieceP));
        changeGH(pieceP, EMPTY);
        return true;
    }
    return false;
}

void GameBoard::changeGH(Coordinates cd, Piece c) {
    if (cd.x < gSize && cd.y < gSize && cd.x >= 0 && cd.y >= 0)
        ground[cd.x][cd.y] = c;
}


bool GameBoard::remove(Coordinates taw) {
    if (!isEmpty(taw)){
        changeGH(taw, EMPTY);
        return true;
    }
    return false;
}

bool GameBoard::makeKing(Coordinates piece) {
    if (getPieceSide(piece) == A_WHITE){
        changeGH(piece, WHITE_KING);
    }
    else {
        changeGH(piece, BLACK_KING);
    }
    return true;
}

void GameBoard::undoKing(Coordinates cd) {
    if (getPiece(cd) == WHITE_KING){
        changeGH(cd, WHITE);
    }
    else if (getPiece(cd) == BLACK_KING){
        changeGH(cd, BLACK);
    }
}

bool GameBoard::isWhiteSpace(Coordinates cd) {
    if ((cd.x + cd.y) % 2 != 0 )
        return false;
    return true;
}

void GameBoard::resetPlayGround() {
    int emptyArea = gSize/4;
    int start = gSize/2 - emptyArea/2;
    int end = start + emptyArea - 1;
    for (int i = 0; i < start; ++i) {
        for (int j = 0; j < gSize; ++j) {
            Coordinates cd{i , j};
            if(isWhiteSpace(cd))
                changeGH(cd, WHITE);
            else
                changeGH(cd, EMPTY);

        }
    }

    for (int i = start; i <= end; ++i) {
        for (int j = 0; j < gSize; ++j) {
            Coordinates cd{i , j};
            changeGH(cd, EMPTY);
        }
    }


    for (int i = end + 1; i < gSize; ++i) {
        for (int j = 0; j < gSize; ++j) {
            Coordinates cd{i , j};
            if(isWhiteSpace(cd))
                changeGH(cd, BLACK);
            else
                changeGH(cd, EMPTY);

        }
    }
}

char GameBoard::PieceToChar(Piece t) {
    switch (t) {
        case EMPTY:
            return EMPTY_C;
        case WHITE:
            return WHITE_C;
        case WHITE_KING:
            return WHITE_KING_C;
        case BLACK:
            return BLACK_C;
        case BLACK_KING:
            return BLACK_KING_C;
        default:
            return '\0';
    }
}

bool GameBoard::isKing(Coordinates cd) {
    Piece p = getPiece(cd);
    if (p == BLACK_KING || p == WHITE_KING)
        return true;
    return false;
}

int GameBoard::getWhiteCounts() {
    int result = 0;
    for (int i = 0; i < gSize; ++i)
        for (int j = 0; j < gSize; ++j)
            if (ground[i][j] == WHITE || ground[i][j] == WHITE_KING)
                result++;
    return result;
}

int GameBoard::getBlackCounts() {
    int result = 0;
    for (int i = 0; i < gSize; ++i)
        for (int j = 0; j < gSize; ++j)
            if (ground[i][j] == BLACK || ground[i][j] == BLACK_KING)
                result++;
    return result;
}

Piece GameBoard::getPiece(Coordinates cd) {
    if (cd.x < gSize && cd.x >= 0 && cd.y < gSize && cd.y >= 0)
        return ground[cd.x][cd.y];
    else
        return NON;
}

int GameBoard::getPlayGroundSize() const {
    return gSize;
}

Side GameBoard::getPieceSide(Coordinates cd) {
    Piece p = getPiece(cd);
    if (p == WHITE || p == WHITE_KING)
        return A_WHITE;
    else if (p == BLACK || p == BLACK_KING)
        return B_BLACK;
    return NONE;
}

bool GameBoard::isPiece(Coordinates cd) {
    if (getPiece(cd) == EMPTY)
        return false;
    return true;
}

