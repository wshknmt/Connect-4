
#include "Board.h"

Board* Board::pInstance = nullptr;

Board::Board()
{
    for (int i = 0; i < HEIGHT; i++) {
        std::vector<int> v1;
        for (int j = 0; j < WIDTH; j++) {
            int field = 0;
            v1.push_back(field);
        }
        fields.push_back(v1);
    }

    for (int i = 0; i < WIDTH; i++) {
        int colOccupancy = 0;
        columnOccupancy.push_back(colOccupancy);
    }

}

Board* Board::getInstance() {
    if (!pInstance)
        pInstance = new Board();
    return pInstance;
}

void Board::print() {
    for (unsigned int i = 0; i < HEIGHT; i++) {
        for (unsigned int j = 0; j < WIDTH; j++) {
            std::cout << fields[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout <<"Occupancy: "<< std::endl;
    for (unsigned int j = 0; j < WIDTH; j++) {
        std::cout << fields[0][j] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

std::vector <std::vector<int> > Board::getFields() {
    return fields;
}
int Board::getWidth() {
    return WIDTH;
}
int Board::getHeight() {
    return HEIGHT;
}

void Board::setField(int row, int col, int value) {
    fields[row][col] = value;
}

bool Board::isColumnFree(int col) {
    if (fields[0][col] == 0) return true;
    else return false;
}
int Board::getFreeRowInColumn(int col) {
    return HEIGHT - columnOccupancy[col] - 1;
}

int Board::getLastDroppedRowInColumn(int col) {
    return HEIGHT - columnOccupancy[col];
}

int Board::dropTokenToColumn(int col, int token) {
    int row = getFreeRowInColumn(col);
    fields[row][col] = token;
    columnOccupancy[col]++;
    return row;
}

bool Board::checkWin(int col, int row, int token) {
    for (int i = 0; i < 8; ++i) {
        winPositions.clear();
        winPositions.push_back(std::make_pair(col, row));
        winCounter = 1;
        checkDirection(col + directions[i][0], row + directions[i][1], i, token);
        checkDirection(col + directions[(i + 4) % 8][0], row + directions[(i + 4) % 8][1], (i + 4) % 8, token);
        if (winCounter >= 4) return true;
    }
    return false;
}

void Board::checkDirection(int col, int row, int direction, int token) {
    if (col < 0 || row < 0 || col >= WIDTH || row >= HEIGHT) return;
    if (fields[row][col] == token) {
        winPositions.push_back(std::make_pair(col, row));
        winCounter++;
        checkDirection(col + directions[direction][0], row + directions[direction][1], direction, token);
    }
}

int Board::getPlayerToMove() {
    return playerToMove;
}

void Board::changePlayerToMove() {
    playerToMove = playerToMove % 2 + 1;
}

std::vector <std::pair<int, int>> Board::getWinPositions() {
    return winPositions;
};

void resetBoard() {

}

void Board::setBotMode(int mode) {
    botMode = mode;
}
int Board::getBotMode() {
    return botMode;
}
