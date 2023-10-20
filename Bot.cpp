#include "Bot.h"

Bot* Bot::pInstance = nullptr;

Bot::Bot()
{

}

int Bot::botTurn() {

    int column = getMove();
    wait();
    Board::getInstance()->dropTokenToColumn(column, Board::getInstance()->getPlayerToMove());
    return column;
}

Bot* Bot::getInstance() {
    if (!pInstance)
        pInstance = new Bot();
    return pInstance;
}

int Bot::getMove() {
    switch(Board::getInstance()->getBotMode()) {
    case 1:
        return getRandomMove();
        break;
    case 2:
        return getMinMaxMove();
        break;
    }
    return 999;

}

int Bot::getRandomMove() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 6);
    return distribution(generator);
}

int Bot::getMinMaxMove() {
    if (Board::getInstance()->checkDraw() )
        return 0;
}

void Bot::wait()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.5, 0.6);
    QTime dieTime = QTime::currentTime().addSecs(distribution(generator));
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
