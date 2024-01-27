#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "PairInt64.h"


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
    int getLastColumn();
    PairInt64 hashCurrentPosition();
    void addHashToMap(PairInt64 index, int ScoreValue);
    bool checkHashInMap(PairInt64 index);
    int getScoreFromMap(PairInt64 index);
    void printTotalHashTime();
    void printTotalSearchTime();
    int getColumnOrder(int num);
    void saveTranspositionTableToFile();
    void loadTranspositionTableFromFile();

    static const int MAX_TEST_COLUMN = 6;
    bool LOAD_TRANSPOSITION_TABLE = true;
    bool SAVE_TRANSPOSITION_TABLE = false;

private:
    static constexpr int WIDTH = 7;
    static constexpr int HEIGHT = 6;
    int directions[8][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int playerToMove = 1;
    int movesCounter = 0;
    int allMovesCounter = 0;
    int lastColumn;
    std::chrono::duration<double> totalHashTime = std::chrono::duration<double>(0);
    std::chrono::duration<double> totalSearchTime = std::chrono::duration<double>(0);

    std::vector <std::vector<int> > fields;
    std::vector <int> columnOccupancy;
    std::vector <std::pair<int, int>> winPositions;
    int winCounter = 0;
    static Board* pInstance;
    std::unordered_map <PairInt64, int> transpositionTable;
    int columnOrder[MAX_TEST_COLUMN];

};

#endif // BOARD_H
