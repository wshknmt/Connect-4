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
    bool isColumnFree(int col);
    int dropTokenToColumn(int col, int token);
    bool checkWin(int col, int row, int token);
    void checkDirection(int col, int row, int direction, int token);
    int getPlayerToMove();

private:
    static constexpr int WIDTH = 7;
    static constexpr int HEIGHT = 6;
    int directions[8][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int playerToMove = 1;

    std::vector <std::vector<int> > fields;
    std::vector <int> columnOccupancy;
    int winCounter = 0;
    static Board* pInstance;
};

#endif // BOARD_H
