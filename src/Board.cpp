
#include "Board.h"

Board* Board::pInstance = nullptr;

Board::Board() {
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

    for(int i = 0; i < MAX_TEST_COLUMN; i++)
        columnOrder[i] = MAX_TEST_COLUMN / 2 + ( 1 - 2 * (i % 2)) * (i + 1) / 2;

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
        std::cout << columnOccupancy[j] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // printTranspositionTable();
    std::cout<<"Points of 1 player: "<< getPointResult(1) << std::endl;
    std::cout<<"Points of 2 player: "<< getPointResult(2) << std::endl;

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

int Board::getNumOfFields() {
    return MAX_TEST_COLUMN * HEIGHT;
    // return WIDTH * HEIGHT;
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
    movesCounter++;
    allMovesCounter++;
    lastColumn = col;
    return row;
}

void Board::removeLastTokenFromColumn(int col) {
    int row = getLastDroppedRowInColumn(col);
    fields[row][col] = 0;
    columnOccupancy[col]--;
    movesCounter--;
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

int Board::getNextPlayerToMove() {
    return playerToMove % 2 + 1;
}

void Board::changePlayerToMove() {
    playerToMove = playerToMove % 2 + 1;
}

std::vector <std::pair<int, int>> Board::getWinPositions() {
    return winPositions;
}

void Board::resetBoard() {
    for (unsigned int i = 0; i < HEIGHT; i++)
        for (unsigned int j = 0; j < WIDTH; j++)
            fields[i][j] = 0;

    for (unsigned int j = 0; j < WIDTH; j++) {
        columnOccupancy[j] = 0;
    }
    movesCounter = 0;
    allMovesCounter = 0;
    winPositions.clear();
    playerToMove = 1;
}

int Board::getMovesCounter() {
    return movesCounter;
}

int Board::getAllMovesCounter() {
    return allMovesCounter;
}

bool Board::checkDraw() {
    if (getNumOfFields() == movesCounter)
        return true;
    else
        return false;
}

int Board::getLastColumn() {
    return lastColumn;
}

PairInt64 Board::hashCurrentPosition() {

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t boardColor = 0; // 1 - red, 0 - blue
    uint64_t boardOccupancy = 0; // 1 - occupied position, 0 - empty position

    for (unsigned int i = 0; i < HEIGHT; i++) {
        for (unsigned int j = 0; j < MAX_TEST_COLUMN; j++) {
        // for (unsigned int j = 0; j < WIDTH; j++) {
            if (fields[i][j] == 1) {
                boardOccupancy |= (1ULL << (i * 7 + j) );
                boardColor     |= (1ULL << (i * 7 + j) );
            } else if (fields[i][j] == 2) {
                boardOccupancy |= (1ULL << (i * 7 + j) );
            }
        }
    }

    PairInt64 pair;
    pair.boardColor = boardColor;
    pair.boardOccupancy = boardOccupancy;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    totalHashTime += duration;

    return pair;
}

void Board::addHashToMap(PairInt64 index, int ScoreValue) {
    transpositionTable.insert({index, ScoreValue});
}

bool Board::checkHashInMap(PairInt64 index) {
    auto start = std::chrono::high_resolution_clock::now();
    auto it = transpositionTable.find(index);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    totalSearchTime += duration;
    if (it != transpositionTable.end())
        return true;
    return false;

}

int Board::getScoreFromMap(PairInt64 index) {
    auto start = std::chrono::high_resolution_clock::now();
    auto it = transpositionTable.find(index);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    totalSearchTime += duration;
    if (it != transpositionTable.end())
        return it->second;
    return 99999;

}

void Board::printTotalHashTime() {
    std::cout << "Total time taken to hash: " << totalHashTime.count() << " seconds" << std::endl;
}

void Board::printTotalSearchTime() {
    std::cout << "Total time taken to search in map: " << totalSearchTime.count() << " seconds" << std::endl;
}

int Board::getColumnOrder(int num) {
    return columnOrder[num];
}

int Board::getWeight(int num) {
    return weights[num];
}

void Board::saveTranspositionTableToFile() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath.parent_path().parent_path().parent_path();
    targetPath += "\\data\\transposition_table";

    std::ofstream outFile(targetPath, std::ios::binary);

    if (outFile.is_open()) {
        for (const auto& entry : transpositionTable) {
            outFile.write(reinterpret_cast<const char*>(&entry.first), sizeof(PairInt64));
            outFile.write(reinterpret_cast<const char*>(&entry.second), sizeof(int));
        }
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing: " << targetPath << std::endl;
    }
}

void Board::loadTranspositionTableFromFile() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath.parent_path().parent_path().parent_path();
    targetPath += "\\data\\transposition_table";
    std::ifstream inFile(targetPath, std::ios::binary);

    if (inFile.is_open()) {
        while (true) {
            PairInt64 key;
            int value;

            inFile.read(reinterpret_cast<char*>(&key), sizeof(PairInt64));
            inFile.read(reinterpret_cast<char*>(&value), sizeof(int));

            if (inFile.eof()) {
                break;
            }

            transpositionTable[key] = value;
        }

        inFile.close();
    } else {
        std::cerr << "Error opening file for reading: " << targetPath << std::endl;
    }
}

void Board::printBinary(int64_t num) {
    std::bitset<64> binary(num);
    std::cout << binary;
}

void Board::printTranspositionTable() {
     for (auto it = transpositionTable.begin(); it != transpositionTable.end(); ++it) {
        const PairInt64& key = it->first;
        int value = it->second;
        std::cout << "Value: " << value <<", Key: {";
        printBinary(key.boardColor);
        std::cout << ", ";
        printBinary(key.boardOccupancy);
        std::cout << std::endl;
    }
}

int Board::getPointResult(int player){
    int points = 0;
    for (unsigned int row = 0; row < HEIGHT; row++) {
        for (unsigned int col = 0; col < MAX_TEST_COLUMN; col++) {
            if (fields[row][col] == player) {
                points += getResults(player, col, row);
            }
        }
    }
    return points;
}

int Board::getResults(int token, int col, int row) {
    int result = 0;
    for (int i = 0; i < 8; ++i) {
        lineCounter = 1;
        allLineCounter = 1;
        checkLineInDirection(col + directions[i][0], row + directions[i][1], i, token);
        if (allLineCounter >= 4)
            result += weights[lineCounter];
    }
    return result;
}

void Board::checkLineInDirection(int col, int row, int direction, int token) {
    if (allLineCounter >= 4) return;
    if (col < 0 || row < 0 || col >= WIDTH || row >= HEIGHT) {
        allLineCounter = 0;
        lineCounter = 0;
        return;
    }
    if (fields[row][col] == token) {
        lineCounter++;
        allLineCounter++;
        checkLineInDirection(col + directions[direction][0], row + directions[direction][1], direction, token);
    }
    else if (fields[row][col] == 0) {
        allLineCounter++;
        checkLineInDirection(col + directions[direction][0], row + directions[direction][1], direction, token);
    }
}