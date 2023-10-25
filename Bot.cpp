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
    std::cout<<"mode: "<<Board::getInstance()->getBotMode()<<std::endl<<std::endl;
    switch(Board::getInstance()->getBotMode()) {
    case 1:
        return getRandomMove();
        break;
    case 2:
        std::cout<<"wszedl: "<<std::endl;
        return getMinMaxMove();
        break;
    default:
        return 999;
    }

}

int Bot::getRandomMove() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 6);
    return distribution(generator);
}

int Bot::getMinMaxMove() {
    int xxx = 9;
    qDebug() << xxx;
    std::cout<<"poczatek"<<std::endl;
    Board::getInstance()->print();
    if (Board::getInstance()->checkDraw() )
        return 0;
    std::cout<<"no draw"<<std::endl;
    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        std::cout<<"warunki dla i = "<<i<<std::endl;
        std::cout<<"isColumnFree: "<<Board::getInstance()->isColumnFree(i)<<std::endl;
        std::cout<<"getFreeRowInColumn "<<Board::getInstance()->getFreeRowInColumn(i)<<std::endl;
        std::cout<<"getPlayerToMove "<<Board::getInstance()->getPlayerToMove()<<std::endl;
        std::cout<<"checkWin "<<Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove())<<std::endl;
        if( Board::getInstance()->isColumnFree(i) && Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove()))
            return (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
    }

    int bestScore = -Board::getInstance()->getNumOfFields();
    std::cout<<"ok minmax: "<<std::endl;

    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if ( Board::getInstance()->isColumnFree(i) ) {
            Board::getInstance()->dropTokenToColumn(i, Board::getInstance()->getPlayerToMove());
            Board::getInstance()->changePlayerToMove();
            int newScore = -getMinMaxMove();
            if ( newScore > bestScore) bestScore = newScore;
            Board::getInstance()->changePlayerToMove();
            Board::getInstance()->dropTokenToColumn(i, Board::getInstance()->getPlayerToMove());
        }
    }
    return bestScore;
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
