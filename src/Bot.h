#ifndef BOT_H
#define BOT_H

#include <random>
#include <algorithm>
#include <utility>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "Board.h"
#include "PairInt64.h"


class Bot
{
public:
    static Bot* getInstance();
    Bot();
    int botTurn();
    int getMove();
    int getRandomMove();
    std::pair<int, int> getMinMaxMove(int alpha, int beta);
    void setMode(int mode);
    int getMode();
    void wait();
    void resetBot();
    void play(int column);
    void undoPlay(int column);
    int getHeuristicMove();
    void printCheckedColumns();
    void columnInfo(int i, int score);

    std::pair<int, int> getHeuristicMinMaxMove(int depth, bool maximizingPlayer);
    std::pair<int, int> evaluateHeuristicMinMax();

    static const int MAX_TOKENS_TO_HASH_IN_TABLE = 34;

private:
    static Bot* pInstance;
    int mode;
    int searchDepth = 0;
    int printNum = 1;
    std::vector<std::pair<int, int>> checkedColumns;
    bool useTT = true;
};

#endif // BOT_H
