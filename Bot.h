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

private:
    static Bot* pInstance;
    int mode;

    int getMove();
    int getRandomMove();
    int getMinMaxMove();
    void wait();
};

#endif // BOT_H
