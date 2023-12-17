#include <conio.h>
#include "GameBoard.h"
#include "CheckersGame.h"
#include "staticFunctions.h"


using namespace std;



int main() {
    staticFunctions::sayHello();

    CheckersGame checkersGame(staticFunctions::getPlayGroundSize());//There is 2 constructors, if you don't enter the playBoardSize OR invalid(negative int), it will be 8 by default;
    checkersGame.setStarterSide(staticFunctions::getStarterSide());//which side starts the game? black or white?
    string finalResult;
    staticFunctions::handleDivider();
    do {

        checkersGame.printGround();
        checkersGame.printAvailablePiecesToMove();
        checkersGame.printAvailableMoves();
        checkersGame.printTurnGuider();

        string wasterOfMemory;
        getline(cin, wasterOfMemory);
        Coordinates cds[2];

        string result = staticFunctions::getLineAndExtractCoordinatesOrCommand(checkersGame, wasterOfMemory, cds);

        staticFunctions::handleDivider();//see the function, here we better clear the console (which i can't do this IN Clion)

        if (result.empty()){
            checkersGame.playRound(cds[0], cds[1]);
        }
        else if(result == EXIT_CM_C){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SHOW_INFO_COLOR);
            cout << "Game ended on your command!" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);

            return 0;
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SHOW_INFO_COLOR);
            cout << result;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
        }

    } while(!checkersGame.checkWinner(finalResult));


    checkersGame.printGround();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NOTICE_COLOR);
    cout << finalResult << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);

    getch();
    return 0;
}
