#ifndef BOT_H
#define BOT_H

#include <random>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "Board.h"


class Bot
{
public:
    static Bot* getInstance();
    Bot();
    int botTurn();
    int getMove();
    int getRandomMove();
    int getMinMaxMove(int alpha, int beta);
    int getMinMaxScore(int alpha, int beta);
    void setMode(int mode);
    int getMode();
    void wait();
    void resetBot();
    void play(int column);
    void undoPlay(int column);

private:
    static Bot* pInstance;
    int mode;
    int recursiveLevel = 0;
};

#endif // BOT_H
