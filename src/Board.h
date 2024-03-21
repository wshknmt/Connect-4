#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <bitset>

#include "PairInt64.h"
#include "GameCustomization.h"

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
    void setPlayerToMove(int player);
    int getNextPlayerToMove();
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
    int getWeight(int num);
    void saveTranspositionTableToFile();
    void loadTranspositionTableFromFile();
    void printBinary(int64_t num);
    void printTranspositionTable();
    int getPointResult(int player);
    void checkLineInDirection(int col, int row, int direction, int token);
    int getResults(int player, int col, int row);
    GameCustomization loadFile(std::ifstream& file);
    bool validateMode(char mode);
    bool validatePlayers(char p1, char p2, int gameMode);
    bool validateField(char field);
    bool validateTokenAmount(int amountP1, int amountP2);
    bool validateBoardStructure();
    bool isTokenInField(int j, int i);
    void addElementToMovesHistory(int value);
    int popBackMovesHistory();
    int movesHistorySize();
    void clearHistorySize();

    static const int MAX_TEST_COLUMN = 7;
    bool LOAD_TRANSPOSITION_TABLE = false;
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
    int lineCounter = 0;
    int allLineCounter = 0;
    static Board* pInstance;
    std::unordered_map <PairInt64, int> transpositionTable;
    int columnOrder[MAX_TEST_COLUMN];
    int weights[5] = {0, 1, 10, 50, 500};
    std::string message;
    std::vector <int> movesHistory;

};

#endif // BOARD_H
