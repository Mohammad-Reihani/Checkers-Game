//
// Created by phoenix on 1/19/2023.
//

#include "staticFunctions.h"


Side staticFunctions::getStarterSide() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), REQ_INFO_COLOR);
    string stringSide;
    cout << "Which side is gonna start this game? (for white : [w-white-0] and for black : [b-black-1])" << endl;
    getline(cin, stringSide);
    stringSide = extractNextElementFromLine(stringSide);
    for (char& x : stringSide)
        x = (char)tolower(x);
    while (!(stringSide == "w" || stringSide == "white" || stringSide == "0" || stringSide == "b" || stringSide == "black" || stringSide == "1")){
        cout << "What did you mean? (for white : [w-white-0] and for black : [b-black-1])" << endl;
        getline(cin, stringSide);
        stringSide = extractNextElementFromLine(stringSide);
        for (char& x : stringSide)
            x = (char)tolower(x);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    if (stringSide == "w" || stringSide == "white" || stringSide == "0")
        return A_WHITE;
    return B_BLACK;
}

string staticFunctions::extractNextElementFromLine(string &line) {
//    string element = "";
    string element;
    int flag = 0;
//    string newline = "";
    string newline;

    //Removing space from the beginning of the line :
    while (line[0] == ' ') {
        line.erase(0, 1);
    }
    //Extracting element from row :
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == ' ') {
            flag = i;
            break;
        }
        element += (char) tolower(line[i]); //make inputs lowercase
    }

    //Getting command out of the row using newline:
    int hold = 0;
    if (element.length() != line.length()) {
        for (int i = flag + 1; i < line.length(); ++i) {
            if (hold == 1 || line[i] != ' ') {
                newline += line[i];
                hold = 1;
            }
        }
    }

    //Removing space from the beginning of the line again :
    while (line[0] == ' ') {
        line.erase(0, 1);
    }
    line = newline;

    return element;
}

bool staticFunctions::isCommand(string basicString) {
    for (char &x : basicString)
        x = (char)tolower(x);
    if (basicString == RESET_CM_C || basicString == REDO_CM_C || basicString == EXIT_CM_C || basicString == REPORT_CM_C)
        return true;
    return false;
}

string staticFunctions::getLineAndExtractCoordinatesOrCommand(CheckersGame &checkersGame, string line, Coordinates cds[]){
    string c1_string = extractNextElementFromLine(line);

    //check if c1 is a command;
    if (isCommand(c1_string)){
        string ans = doCommand(checkersGame, c1_string);
        return ans;
    }
    extractNextElementFromLine(line);
    string c2_string = extractNextElementFromLine(line);
    if (c1_string.empty() || c2_string.empty() || checkersGame.isNotCoordinates(c1_string) || checkersGame.isNotCoordinates(c2_string))
        return "There is a problem with your move OR command!\n(consider that the move command format must be \"%Coordinated% to %Coordinates%\" (Coordinates : '1^size'+'a^...'))\n";
    else {
        cds[0] = stringToCoordinates(c1_string);
        cds[1] = stringToCoordinates(c2_string);
        return "";
    }
}

string staticFunctions::doCommand(CheckersGame &checkersGame, const string& cm) {
    string ans = "UNDefined command!\n";
    if (cm == RESET_CM_C) {
        checkersGame.resetTheGame(getStarterSide());
        ans = "Resetting the game...\nDone\n";
    }
    else if (cm == REDO_CM_C){
        ans = "Checking if redo is valid...\n";
        ans += checkersGame.redoIfPossible();
    }
    else if (cm == EXIT_CM_C){
        ans = EXIT_CM_C;
    }
    else if (cm == REPORT_CM_C){
        ans = "Game report 'tll now :\n";
        ans += checkersGame.giveReport();
    }
    return ans;
}

int staticFunctions::getPlayGroundSize(){
    int result = -1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), REQ_INFO_COLOR);
    cout << "Please enter the play ground size that you prefer (e.g. 8 for (8x8), 10 for (10x10), 12 for (12x12), ...) :" << endl;
    while (result < 5 || result > 26){
        try {
            string strSize;
            getline(cin, strSize);
            strSize = extractNextElementFromLine(strSize);
            result = std::stoi(strSize);

            if (result < 5 || result > 26) {throw logic_error("number range not allowed!");}

            if (digitsCount(result) != strSize.length()){result = 0; throw logic_error("not completely number!");}
        }
        catch (...) {
            cout <<"you must enter a NUMBER and it should be more than 4 and less than 27 :" << endl;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
    return result;
}

void staticFunctions::sayHello(){
    handleDivider();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SHOW_INFO_COLOR);
    cout << "Hi, welcome to my checkers game." << endl;
    cout << "This game is written by Mohammad Reza Reihani" << endl;
    cout << "   (EE student at IUST - final project for programming basics - teacher : dr. Ashtiani)" << endl;
    cout << endl;
    cout << "First you need to choose the game board size!(i think this is a pretty fun AND useful option) and then" << endl;
    cout << "   after choosing the starter side (black or white), "<< endl;
    cout << "   play the game using commands!(see readme.txt file for full description)" << endl;
    cout << endl;
    cout << "Available Commands (commands are not key sensitive) :" << endl;
    cout << "   Moving   command : e.g. 3c to 4d" << endl;
    cout << "   'reset'  command : completely resets the game" << endl;
    cout << "   'redo'   command : redo the last move via this command! this is so cool. please try this" << endl;
    cout << "   'report' command : shows a report/some information of the current game! you should definitely try this" << endl;
    cout << "   'exit'   command : completely closes the game. " << endl;
    cout << "       if you got tired OR you choose a very big ground size you can use this command" << endl;
    cout << endl;
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}

void staticFunctions::handleDivider() {
//    system("CLS");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DIVIDER_COLOR);
    cout << "----------------------------------------------------------------------------------------" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}

int staticFunctions::digitsCount(int input) {
    int result = 0;
    while(input > 0){
        result++;
        input /= 10;
    }
    return result;
}

Coordinates staticFunctions::stringToCoordinates(string cString) {
    int x = 0;
    int holder;
    for (int i = 0; cString[i] < 58 && cString[i] > 47; i++){
        x = x*10 + (cString[i] - 48);
        holder = i;
    }
    x--;
    int y = tolower(cString[holder + 1]) - 97;
    Coordinates cd{x, y};
    return cd;
}

string staticFunctions::getMoveErrorByCode(int error_code) {

    switch (error_code) {
        case 0:
            return "Start piece empty";
        case 1:
            return "Side does not match the selected piece";
        case 2:
            return "Destination Is Not Empty";
        case 3:
        case 4:
            return "Only White Squares Are Allowed";
        case 5:
            return "Backward Move NOT Allowed (Except for KINGs)";
        case 6:
            return "Jump Distance Limit Exceeded";
        default:
            return "!NOT specified!";
    }
}

string staticFunctions::sideToString(Side side) {
    if (side == A_WHITE){
        string s = "White(represented by ";
        return  s + WHITE_C + ")";
    }
    else if (side == B_BLACK){
        string s = "Black(represented by \'";
        return  s + BLACK_C + "\')";
    }
    return "!!!";
}

bool staticFunctions::isForward(Side side, Coordinates from, Coordinates to) {
    if ((side == A_WHITE && from.x < to.x) || (side == B_BLACK && from.x > to.x))
        return true;
    return false;
}

bool staticFunctions::checkCoordinatedDistance(Coordinates from, Coordinates to, int limit) {
    int holder = abs(from.x - to.x);
    int holder1 = abs(from.y - to.y);
    if (holder > limit || holder1 > limit)
        return true;
    return false;
}

string staticFunctions::CoordinatesToConsoleInput(Coordinates cd) {
    string result = to_string(cd.x + 1) + POS[cd.y];
    return result;
}

void staticFunctions::printTos(vector<Coordinates> d) {
    cout << " to { ";
    cout << staticFunctions::CoordinatesToConsoleInput(d[0]);
    d.erase(d.begin());
    for (auto & x : d) {
        cout << ", " << staticFunctions::CoordinatesToConsoleInput(x);
    }
    cout << "}";
}
