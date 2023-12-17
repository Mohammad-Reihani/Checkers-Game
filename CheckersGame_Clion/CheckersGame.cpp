//
// Created by phoenix on 1/19/2023.
//

#include "CheckersGame.h"
#include "staticFunctions.h"


CheckersGame::CheckersGame() {
    CheckersGame m(8);
}

CheckersGame::CheckersGame(int gameBoardSize) {
    GameBoard ground(gameBoardSize);
    playGround = ground;
}

void CheckersGame::playRound(Coordinates from, Coordinates to) {
    Side oppositeSide;
    if (currentSide == A_WHITE)
        oppositeSide = B_BLACK;
    else
        oppositeSide = A_WHITE;


    vector<bool> errorHolder(7, false);
    if ((errorHolder[0] = playGround.isEmpty(from)) ||
        (errorHolder[1] = sideDoesNotMatch(currentSide, from)) ||
        (errorHolder[2] = !playGround.isEmpty(to)) ||
        (errorHolder[3] = !GameBoard::isWhiteSpace(from)) || (errorHolder[4] = !GameBoard::isWhiteSpace(to)) ||
        (errorHolder[5] = (!staticFunctions::isForward(currentSide, from, to) && !playGround.isKing(from))) ||
        (errorHolder[6] = staticFunctions::checkCoordinatedDistance(from, to, 2))//this should not be placed here!(it may be hit another piece)
            )
    {
        int error;
        for (int i = 0; i < errorHolder.size(); ++i)
            if (errorHolder[i])
                error = i;
        string error_message = staticFunctions::getMoveErrorByCode(error);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NOTICE_COLOR);
        cout << "ERROR : " << error_message << " \nTry again.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);

        return;
    } else {
        //now the move is safe;

        updateMovablePieces(currentSide);
        updateInHittingPositionPieces();

        if(((PiecesThatCanHit_white.empty() && currentSide == A_WHITE) || (PiecesThatCanHit_black.empty() && currentSide == B_BLACK)) && isMovable(from, currentSide) &&
            !staticFunctions::checkCoordinatedDistance(from, to, 1))
        {
            if (playGround.isKing(from))
                addKingMovesAndUpdateCoordinates(from, to);
            playGround.moveTo(from, to);

            checkAndSetKings();
            clearHM();

            //recording the move for future uses :
            recordedMoves.push_back(DoubleCoordinates{from, to});

            //recording side and possibly deleted pic for future uses :
            recordedSides.push_back(currentSide);
            recordedDeletedPieces.push_back(DeletedOnTurn{Coordinates{-1, -1}, EMPTY });

            //counting moves for the sake of future uses :
            if (currentSide == A_WHITE)
                countWhiteMoves++;
            else
                countBlackMoves++;
            countTotalMoves++;


            currentSide = oppositeSide;
            return;
        }
        else if(checkCrossJumpAndEmpty(from, to) && canHit(from, currentSide) && staticFunctions::checkCoordinatedDistance(from, to, 1)){
            Piece savePiece;
            Coordinates mid{(from.x + to.x)/2, (from.y + to.y)/2};

            if (playGround.isKing(from))
                addKingMovesAndUpdateCoordinates(from, to);

            playGround.moveTo(from, to);
            savePiece = playGround.getPiece(mid);
            playGround.remove(mid);


            //recording the move for the sake of future uses :
            recordedMoves.push_back(DoubleCoordinates{from, to});

            //recording side and possibly deleted pic for future uses :
            recordedSides.push_back(currentSide);
            recordedDeletedPieces.push_back(DeletedOnTurn{mid, savePiece });

            //counting moves for the sake of future uses :
            if (currentSide == A_WHITE)
                countWhiteMoves++;
            else
                countBlackMoves++;
            countTotalMoves++;

            checkAndSetKings();
            //the below update is critical :
            updateInHittingPositionPieces();
            if ((currentSide == A_WHITE && PiecesThatCanHit_white.empty()) || (currentSide == B_BLACK && PiecesThatCanHit_black.empty()) ||
                !canHit(to, currentSide)//in the last condition, you decide if there is two different options in a turn for a currentSide, you can only choose one of them
                    )
            {
                clearHM();
                currentSide = oppositeSide;
                return;
            }
            clearHM();
            return;
        }

        cout << "move against rolls!\n(you may have a chance to hit! - see line 50-110)" << endl;
        return;
    }
}

bool CheckersGame::sideDoesNotMatch(Side side, Coordinates coordinates) {
    if ((side == A_WHITE && (playGround.getPiece(coordinates) == WHITE || playGround.getPiece(coordinates) == WHITE_KING)) ||
        (side == B_BLACK && (playGround.getPiece(coordinates) == BLACK || playGround.getPiece(coordinates) == BLACK_KING)))
        return false;
    return true;
}

void CheckersGame::updateMovablePieces(Side side) {
    if (side == A_WHITE) {
        movablePieces_white.clear();
        for (int i = 0; i < playGround.getPlayGroundSize(); ++i)
            for (int j = 0; j < playGround.getPlayGroundSize(); ++j)
                if ((playGround.getPiece(Coordinates{i, j}) == WHITE &&
                     (
                             playGround.getPiece(Coordinates{i + 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i + 1, j - 1}) == EMPTY
                     )
                    )
                    ||
                    (playGround.getPiece(Coordinates{i, j}) == WHITE_KING &&
                     (
                             playGround.getPiece(Coordinates{i + 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i + 1, j - 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i - 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i - 1, j - 1}) == EMPTY
                     )
                    )
                        )
                {
                    movablePieces_white.push_back(Coordinates{i, j});
                }
    }
    else if (side == B_BLACK) {
        movablePieces_black.clear();
        for (int i = 0; i < playGround.getPlayGroundSize(); ++i)
            for (int j = 0; j < playGround.getPlayGroundSize(); ++j)
                if ((playGround.getPiece(Coordinates{i, j}) == BLACK &&
                     (
                             playGround.getPiece(Coordinates{i - 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i - 1, j - 1}) == EMPTY
                     )
                    )
                    ||
                    (playGround.getPiece(Coordinates{i, j}) == BLACK_KING &&
                     (
                             playGround.getPiece(Coordinates{i + 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i + 1, j - 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i - 1, j + 1}) == EMPTY ||
                             playGround.getPiece(Coordinates{i - 1, j - 1}) == EMPTY
                     )
                    )
                        ) {
                    movablePieces_black.push_back(Coordinates{i, j});
                }
    }
}

void CheckersGame::updateInHittingPositionPieces() {
    PiecesThatCanHit_white.clear();
    PiecesThatCanHit_black.clear();
    for (int i = 0; i < playGround.getPlayGroundSize(); ++i) {
        for (int j = 0; j < playGround.getPlayGroundSize(); ++j) {
            Coordinates holder{i, j};
            if (playGround.isPiece(holder)){
                if (playGround.getPieceSide(holder) == A_WHITE){
                    if (pieceCanHit(holder, B_BLACK) || (playGround.isKing(holder) && kingCanHit(holder, B_BLACK)))
                        PiecesThatCanHit_white.push_back(holder);
                }
                else{
                    if (pieceCanHit(holder, A_WHITE) || (playGround.isKing(holder) && kingCanHit(holder, A_WHITE)))
                        PiecesThatCanHit_black.push_back(holder);
                }
            }
        }
    }
}

bool CheckersGame::kingCanHit(Coordinates cd, Side oppositeSide) {
    //its guaranteed that cd is a king(side does not matter) :
    if (
            (playGround.getPieceSide(Coordinates{cd.x + 1, cd.y + 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x + 2, cd.y + 2})) ||
            (playGround.getPieceSide(Coordinates{cd.x + 1, cd.y - 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x + 2, cd.y - 2})) ||
            (playGround.getPieceSide(Coordinates{cd.x - 1, cd.y + 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x - 2, cd.y + 2})) ||
            (playGround.getPieceSide(Coordinates{cd.x - 1, cd.y - 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x - 2, cd.y - 2}))
            )
    {
        return true;
    }
    return false;
}

bool CheckersGame::pieceCanHit(Coordinates cd, Side oppositeSide) {
    if ((oppositeSide == B_BLACK &&
         (
                 (playGround.getPieceSide(Coordinates{cd.x + 1, cd.y + 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x + 2, cd.y + 2})) ||
                 (playGround.getPieceSide(Coordinates{cd.x + 1, cd.y - 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x + 2, cd.y - 2}))
         ))
        ||
        (oppositeSide == A_WHITE &&
         (
                 (playGround.getPieceSide(Coordinates{cd.x - 1, cd.y + 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x - 2, cd.y + 2})) ||
                 (playGround.getPieceSide(Coordinates{cd.x - 1, cd.y - 1}) == oppositeSide && playGround.isEmpty(Coordinates{cd.x - 2, cd.y - 2}))
         ))
            ){
        return true;
    }
    return false;
}

bool CheckersGame::isMovable(Coordinates cd, Side side) {
    if (side == A_WHITE) {
        for (Coordinates &i: movablePieces_white) {
            if (i.x == cd.x && i.y == cd.y)
                return true;
        }
    }
    else {
        for (Coordinates &i: movablePieces_black) {
            if (i.x == cd.x && i.y == cd.y)
                return true;
        }
    }
    return false;
}

void CheckersGame::printGround() {
    playGround.printGround();
}

int CheckersGame::getWhitePiecesCounts() {
    return playGround.getWhiteCounts();
}

int CheckersGame::getBlackPiecesCounts() {
    return playGround.getBlackCounts();
}

bool CheckersGame::canHit(Coordinates cd, Side side) {
    if (side == A_WHITE) {
        for (Coordinates &i: PiecesThatCanHit_white) {
            if (i.x == cd.x && i.y == cd.y)
                return true;
        }
    }
    else {
        for (Coordinates &i: PiecesThatCanHit_black) {
            if (i.x == cd.x && i.y == cd.y)
                return true;
        }
    }
    return false;
}

void CheckersGame::clearHM() {
    PiecesThatCanHit_white.clear();
    PiecesThatCanHit_black.clear();
    movablePieces_white.clear();
    movablePieces_black.clear();
}

void CheckersGame::checkAndSetKings() {
    for (int i = 0; i < playGround.getPlayGroundSize(); ++i) {
        for (int j = 0; j < playGround.getPlayGroundSize(); ++j) {
            Coordinates holder{i, j};
            if (playGround.isPiece(holder) && !playGround.isKing(holder) && ((playGround.getPieceSide(holder) == A_WHITE && i == playGround.getPlayGroundSize()-1) || (playGround.getPieceSide(holder) == B_BLACK && i == 0))){
                playGround.makeKing(holder);
                recordedKingsMoves.push_back(KingMoves{holder, 0});
            }
        }
    }

}

void CheckersGame::printAvailablePiecesToMove() {

    clearHM();
    updateMovablePieces(currentSide);
    updateInHittingPositionPieces();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SHOW_INFO_COLOR);
    cout << "Available pieces to move for " << staticFunctions::sideToString(currentSide) <<  ":\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);

    if (currentSide == A_WHITE){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_SIDE_COLOR);
        if (!PiecesThatCanHit_white.empty()){
            for (auto & i : PiecesThatCanHit_white) {
                cout << staticFunctions::CoordinatesToConsoleInput(i) << ", ";
            }
        } else {
            for (auto & i : movablePieces_white) {
                cout << staticFunctions::CoordinatesToConsoleInput(i) << ", ";
            }
        }
        cout << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    }
    else if (currentSide == B_BLACK){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_SIDE_COLOR);
        if (!PiecesThatCanHit_black.empty()){
            for (Coordinates & i : PiecesThatCanHit_black) {
                cout << staticFunctions::CoordinatesToConsoleInput(i) << ", ";
            }
        } else {
            for (auto & i : movablePieces_black) {
                cout << staticFunctions::CoordinatesToConsoleInput(i) << ", ";
            }
        }
        cout << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    }
    clearHM();
}

void CheckersGame::printAvailableMoves() {
    clearHM();
    updateMovablePieces(currentSide);
    updateInHittingPositionPieces();

    vector<Coordinates> destinations_of_a_piece;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SHOW_INFO_COLOR);
    cout << "Available moves for " << staticFunctions::sideToString(currentSide) <<  ":\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);

    if (currentSide == A_WHITE){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_SIDE_COLOR);
        if (!PiecesThatCanHit_white.empty()){
            for (auto & i : PiecesThatCanHit_white) {
                destinations_of_a_piece = getDestinationsForHitter(i);
                cout << staticFunctions::CoordinatesToConsoleInput(i);
                staticFunctions::printTos(destinations_of_a_piece);
                cout << endl;
            }
        } else {
            for (auto & i : movablePieces_white) {
                destinations_of_a_piece = getDestinationsForNormalMovers(i);
                cout << staticFunctions::CoordinatesToConsoleInput(i);
                staticFunctions::printTos(destinations_of_a_piece);
                cout << endl;
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    }
    else if (currentSide == B_BLACK){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_SIDE_COLOR);
        if (!PiecesThatCanHit_black.empty()){
            for (Coordinates & i : PiecesThatCanHit_black) {
                destinations_of_a_piece = getDestinationsForHitter(i);
                cout << staticFunctions::CoordinatesToConsoleInput(i);
                staticFunctions::printTos(destinations_of_a_piece);
                cout << endl;
            }
        } else {
            for (auto & i : movablePieces_black) {
                destinations_of_a_piece = getDestinationsForNormalMovers(i);
                cout << staticFunctions::CoordinatesToConsoleInput(i);
                staticFunctions::printTos(destinations_of_a_piece);
                cout << endl;
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    }
    cout << endl;
    clearHM();
}

void CheckersGame::resetTheGame(Side starterSide) {
    GameBoard ground(playGround.getPlayGroundSize());
    setStarterSide(starterSide);
    playGround = ground;
}

bool CheckersGame::checkWinner(string &result) {
    updateMovablePieces(currentSide);
    updateInHittingPositionPieces();
    if (getWhitePiecesCounts() == 0) {
        result = staticFunctions::sideToString(B_BLACK) + " won the game!";
        clearHM();
        return true;
    }
    if (getBlackPiecesCounts() == 0) {
        result = staticFunctions::sideToString(A_WHITE) + " won the game!";
        clearHM();
        return true;
    }
    if (movablePieces_black.empty() && movablePieces_white.empty() && PiecesThatCanHit_black.empty() && PiecesThatCanHit_white.empty()){
        result = "No one won the game! DRAW!\n(no move can be made)";
        clearHM();
        return true;
    }
    clearHM();
    return false;
}

void CheckersGame::setStarterSide(Side side) {
    currentSide = side;
}

Side CheckersGame::getCurrentSide() {
    return currentSide;
}

void CheckersGame::printTurnGuider() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), REQ_INFO_COLOR);
    cout << "It's the ";
    cout << staticFunctions::sideToString(currentSide);
    cout << " turn.\nmake your move OR give command :" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}

bool CheckersGame::isNotCoordinates(const string &cString) {
    if (cString.length() < 2 || !(tolower(cString[cString.length() - 1]) > 96 && tolower(cString[cString.length() - 1]) < 123) || !(tolower(cString[0]) > 47 && tolower(cString[0]) < 58))
        return true;
    for (int i = 0; i < cString.length(); i++){
        if ((tolower(cString[i]) > 96 && tolower(cString[i]) < 123) && (tolower(cString[i-1]) > 47 && tolower(cString[i-1]) < 58) && (tolower(cString[i+1]) > 47 && tolower(cString[i+1]) < 58))
            return true;
        if ((tolower(cString[i]) > 47 && tolower(cString[i]) < 58) && (tolower(cString[i-1]) > 96 && tolower(cString[i-1]) < 123) && (tolower(cString[i+1]) > 96 && tolower(cString[i+1]) < 123))
            return true;
    }
    if (staticFunctions::stringToCoordinates(cString).x >= playGround.getPlayGroundSize()  || staticFunctions::stringToCoordinates(cString).y >= playGround.getPlayGroundSize())
        return true;
    return false;
}

string CheckersGame::redoIfPossible() {
    if (countTotalMoves != 0){

        if (playGround.isKing(recordedMoves[countTotalMoves-1].to) && findAndGetKingMoves(recordedMoves[countTotalMoves-1].to) == 0){
            playGround.undoKing(recordedMoves[countTotalMoves-1].to);
            removeKingFromRecorded(recordedMoves[countTotalMoves-1].to);
        }
        else if (playGround.isKing(recordedMoves[countTotalMoves-1].to)){
            removeOneMoveFromKingAndUpdateCd(recordedMoves[countTotalMoves - 1].to, recordedMoves[countTotalMoves - 1].from);
        }

        playGround.moveTo(recordedMoves[countTotalMoves-1].to, recordedMoves[countTotalMoves-1].from);

//        cout << recordedDeletedPieces[countTotalMoves - 1].deleted.x << " " << recordedDeletedPieces[countTotalMoves - 1].deleted.y << endl;
        playGround.changeGH(recordedDeletedPieces[countTotalMoves - 1].deleted, recordedDeletedPieces[countTotalMoves - 1].deletedP);
        currentSide = recordedSides[countTotalMoves-1];

        //deleting the lasts :
        recordedMoves.erase(recordedMoves.end() - 1);
        recordedSides.erase(recordedSides.end() - 1);
        recordedDeletedPieces.erase(recordedDeletedPieces.end() - 1);



//        cout << recordedSides.size() << endl;
//        cout << countTotalMoves << endl;
//        sideToString(currentSide);

        countTotalMoves--;
        if (currentSide == A_WHITE)
            countWhiteMoves--;
        else
            countBlackMoves--;

        return "Redo at your command!\n";
    }
    return "Redo NOT possible(you have made no moves at all!)\n";
}

string CheckersGame::giveReport() {
    string ans = "You are playing on a " + to_string(playGround.getPlayGroundSize()) + "x" + to_string(playGround.getPlayGroundSize()) + " game board\n";
    ans += "Totally " + to_string(countTotalMoves) + " moves has been made by two player(redo is not considered as a move)" + "\n";
    ans += "   white moves (" + staticFunctions::sideToString(A_WHITE) + ") : " + to_string(countWhiteMoves) + "\n";
    ans += "   black moves (" + staticFunctions::sideToString(B_BLACK) + ") : " + to_string(countWhiteMoves) + "\n";
    ans += "Totally " + to_string((getWhitePiecesCounts() + getBlackPiecesCounts())) + " pieces are on the board" + "\n";
    ans += "   white pieces count (" + staticFunctions::sideToString(A_WHITE) + ") : " + to_string(getWhitePiecesCounts()) + "\n";
    ans += "   black pieces count (" + staticFunctions::sideToString(B_BLACK) + ") : " + to_string(getBlackPiecesCounts()) + "\n";

    int temp1 = getWhiteRemovedPiecesCounts();
    int temp2 = getBlackRemovedPiecesCounts();

    ans += "Totally " + to_string((temp1 + temp2)) + " has been removed" + "\n";
    ans += "   white removed pieces count (" + staticFunctions::sideToString(A_WHITE) + ") : " + to_string(temp1) + "\n";
    ans += "   black removed pieces count (" + staticFunctions::sideToString(B_BLACK) + ") : " + to_string(temp2) + "\n";

    ans += "Totally " + to_string(recordedKingsMoves.size()) + " kings are on the ground" + "\n";
    for (auto &i : recordedKingsMoves) {
        ans += to_string(i.cd.x) + "." + to_string(i.cd.y) + " moves: " + to_string(i.moves) + "\n";
    }
//    ans += "   white removed pieces count (" + staticFunctions::sideToString(A_WHITE) + ") : " + to_string(temp1) + "\n";
//    ans += "   black removed pieces count (" + staticFunctions::sideToString(B_BLACK) + ") : " + to_string(temp2) + "\n";

    return ans;
}

int CheckersGame::getWhiteRemovedPiecesCounts() {
    int result = 0;
    for (auto &x : recordedDeletedPieces)
        if (x.deletedP == WHITE || x.deletedP == WHITE_KING)
            result++;
    return result;
}

int CheckersGame::getBlackRemovedPiecesCounts() {
    int result = 0;
    for (auto &x : recordedDeletedPieces)
        if (x.deletedP == BLACK || x.deletedP == BLACK_KING)
            result++;
    return result;
}

void CheckersGame::addKingMovesAndUpdateCoordinates(Coordinates cd, Coordinates to) {
    for (auto &x: recordedKingsMoves) {
        if (x.cd.x == cd.x && x.cd.y == cd.y){
            x.moves++;
            x.cd = to;
        }
    }
}

int CheckersGame::findAndGetKingMoves(Coordinates cd) {
    for (auto &i: recordedKingsMoves) {
        if (i.cd.x == cd.x && i.cd.y == cd.y)
            return i.moves;
    }
    return -1;
}

bool CheckersGame::checkCrossJumpAndEmpty(Coordinates from, Coordinates to) {
    if (from.x == to.x || from.y == to.y || playGround.getPiece(Coordinates{(from.x + to.x)/2, (from.y + to.y)/2}) == EMPTY)
        return false;
    return true;
}

vector<Coordinates> CheckersGame::getDestinationsForHitter(const Coordinates &cd) {
    vector<Coordinates> ans;
    Side oppositeSide;
    if (currentSide == A_WHITE)
        oppositeSide = B_BLACK;
    else
        oppositeSide = A_WHITE;

    if (playGround.isKing(cd)){
        if (!playGround.isEmpty(Coordinates{cd.x - 1, cd.y - 1}) && playGround.isEmpty(Coordinates{cd.x - 2, cd.y - 2}) && playGround.getPieceSide(Coordinates{cd.x - 1, cd.y - 1}) == oppositeSide)
            ans.push_back(Coordinates{cd.x - 2, cd.y - 2});

        if (!playGround.isEmpty(Coordinates{cd.x + 1, cd.y + 1}) && playGround.isEmpty(Coordinates{cd.x + 2, cd.y + 2}) && playGround.getPieceSide(Coordinates{cd.x + 1, cd.y + 1}) == oppositeSide)
            ans.push_back(Coordinates{cd.x + 2, cd.y + 2});

        if (!playGround.isEmpty(Coordinates{cd.x + 1, cd.y - 1}) && playGround.isEmpty(Coordinates{cd.x + 2, cd.y - 2}) && playGround.getPieceSide(Coordinates{cd.x + 1, cd.y - 1}) == oppositeSide)
            ans.push_back(Coordinates{cd.x + 2, cd.y - 2});

        if (!playGround.isEmpty(Coordinates{cd.x - 1, cd.y + 1}) && playGround.isEmpty(Coordinates{cd.x - 2, cd.y + 2}) && playGround.getPieceSide(Coordinates{cd.x - 1, cd.y + 1}) == oppositeSide)
            ans.push_back(Coordinates{cd.x - 2, cd.y + 2});
    }
    else {
        if (playGround.getPieceSide(cd) == A_WHITE) {
            if (!playGround.isEmpty(Coordinates{cd.x + 1, cd.y + 1}) && playGround.isEmpty(Coordinates{cd.x + 2, cd.y + 2}) && playGround.getPieceSide(Coordinates{cd.x + 1, cd.y + 1}) == oppositeSide)
                ans.push_back(Coordinates{cd.x + 2, cd.y + 2});

            if (!playGround.isEmpty(Coordinates{cd.x + 1, cd.y - 1}) && playGround.isEmpty(Coordinates{cd.x + 2, cd.y - 2}) && playGround.getPieceSide(Coordinates{cd.x + 1, cd.y - 1}) == oppositeSide)
                ans.push_back(Coordinates{cd.x + 2, cd.y - 2});
        }
        else if (playGround.getPieceSide(cd) == B_BLACK){
            if (!playGround.isEmpty(Coordinates{cd.x - 1, cd.y - 1}) && playGround.isEmpty(Coordinates{cd.x - 2, cd.y - 2}) && playGround.getPieceSide(Coordinates{cd.x - 1, cd.y - 1}) == oppositeSide)
                ans.push_back(Coordinates{cd.x - 2, cd.y - 2});
            if (!playGround.isEmpty(Coordinates{cd.x - 1, cd.y + 1}) && playGround.isEmpty(Coordinates{cd.x - 2, cd.y + 2}) && playGround.getPieceSide(Coordinates{cd.x - 1, cd.y + 1}) == oppositeSide)
                ans.push_back(Coordinates{cd.x - 2, cd.y + 2});
        }
    }
    return ans;
}

vector<Coordinates> CheckersGame::getDestinationsForNormalMovers(const Coordinates &cd) {
    vector<Coordinates> ans;
    if (playGround.isKing(cd)){
        if (playGround.isEmpty(Coordinates{cd.x - 1, cd.y - 1}))
            ans.push_back(Coordinates{cd.x - 1, cd.y - 1});

        if (playGround.isEmpty(Coordinates{cd.x + 1, cd.y + 1}))
            ans.push_back(Coordinates{cd.x + 1, cd.y + 1});

        if (playGround.isEmpty(Coordinates{cd.x + 1, cd.y - 1}))
            ans.push_back(Coordinates{cd.x + 1, cd.y - 1});

        if (playGround.isEmpty(Coordinates{cd.x - 1, cd.y + 1}))
            ans.push_back(Coordinates{cd.x - 1, cd.y + 1});
    }
    else {
        if (playGround.getPieceSide(cd) == A_WHITE) {
            if (playGround.isEmpty(Coordinates{cd.x + 1, cd.y + 1}))
                ans.push_back(Coordinates{cd.x + 1, cd.y + 1});

            if (playGround.isEmpty(Coordinates{cd.x + 1, cd.y - 1}))
                ans.push_back(Coordinates{cd.x + 1, cd.y - 1});
        }
        else if (playGround.getPieceSide(cd) == B_BLACK){
            if (playGround.isEmpty(Coordinates{cd.x - 1, cd.y - 1}))
                ans.push_back(Coordinates{cd.x - 1, cd.y - 1});
            if (playGround.isEmpty(Coordinates{cd.x - 1, cd.y + 1}))
                ans.push_back(Coordinates{cd.x - 1, cd.y + 1});
        }
    }
    return ans;
}

void CheckersGame::removeKingFromRecorded(Coordinates cd) {
    int hold;
    for (int i = 0; i < recordedKingsMoves.size(); i++) {
        if (recordedKingsMoves[i].cd.x == cd.x && recordedKingsMoves[i].cd.y == cd.y){
            hold = i;
            break;
        }
    }
    recordedKingsMoves.erase(recordedKingsMoves.begin() + hold);
}

void CheckersGame::removeOneMoveFromKingAndUpdateCd(Coordinates cd, Coordinates from) {
    for (auto &x: recordedKingsMoves) {
        if (x.cd.x == cd.x && x.cd.y == cd.y){
            x.moves--;
            x.cd = from;
        }
    }
}
