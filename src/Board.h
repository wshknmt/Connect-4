#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>


class Board
{
public:
    static Board* getInstance();
    Board();
    void print();
    std::vector <std::vector<int> > getFields();
    void setField(int row, int col, int value);
    int getWidth();
    int getHeight();
    int getNumOfFields();
    bool isColumnFree(int col);
    int dropTokenToColumn(int col, int token);
    void removeLastTokenFromColumn(int col);
    bool checkWin(int col, int row, int token);
    void checkDirection(int col, int row, int direction, int token);
    int getPlayerToMove();
    void changePlayerToMove();
    int getFreeRowInColumn(int col);
    int getLastDroppedRowInColumn(int col);
    std::vector <std::pair<int, int>> getWinPositions();
    void resetBoard();
    int getMovesCounter();
    int getAllMovesCounter();
    bool checkDraw();
private:
    static constexpr int WIDTH = 7;
    static constexpr int HEIGHT = 6;
    int directions[8][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int playerToMove = 1;
    int movesCounter = 0;
    int allMovesCounter = 0;

    std::vector <std::vector<int> > fields;
    std::vector <int> columnOccupancy;
    std::vector <std::pair<int, int>> winPositions;
    int winCounter = 0;
    static Board* pInstance;

};

#endif // BOARD_H
