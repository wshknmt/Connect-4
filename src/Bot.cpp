#include "Bot.h"

Bot* Bot::pInstance = nullptr;

Bot::Bot()
{

}

int Bot::botTurn() {

    int column = getMove();
    std::cout<<"probuje wrzucic do kolumny: "<<column<<std::endl<<std::endl;
    wait();
    Board::getInstance()->dropTokenToColumn(column, Board::getInstance()->getPlayerToMove());
    return column;
}

Bot* Bot::getInstance() {
    if (!pInstance)
        pInstance = new Bot();
    return pInstance;
}

void Bot::setMode(int mode) {
    this->mode = mode;
}

int Bot::getMode() {
    return mode;
}

int Bot::getMove() {
    std::cout<<"mode: "<<getMode()<<std::endl<<std::endl;
    switch(getMode()) {
    case 1:
        return getRandomMove();
        break;
    case 2:
        std::cout<<"wszedl: "<<std::endl;
        return getMinMaxMove( -Board::getInstance()->getNumOfFields() / 2, Board::getInstance()->getNumOfFields() / 2);
//        return getMinMaxMove( -1, 1);
        break;
    default:
        return 999;
    }

}

int Bot::getRandomMove() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 6);
    int val = distribution(generator);
    if (Board::getInstance()->isColumnFree(val))
        return val;
    else
        return getRandomMove();
}

int Bot::getMinMaxMove(int alpha, int beta) {
//    std::cout<<"all counter: "<< Board::getInstance()->getAllMovesCounter()<<std::endl;
//    std::cout<<"getMinMaxMove"<<std::endl;
    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(i) && Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove()))
            return i;
    }

//    int max = (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
//    if (beta > max) {
//        beta = max;
//        if (alpha >= beta) return beta;
//    }
//    int bestScore = -Board::getInstance()->getNumOfFields();
    int columnOfBestScore = -1;

    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if ( Board::getInstance()->isColumnFree(i) ) {
            Board::getInstance()->dropTokenToColumn(i, Board::getInstance()->getPlayerToMove());
            Board::getInstance()->changePlayerToMove();
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore >= beta) {
                Board::getInstance()->changePlayerToMove();
                Board::getInstance()->removeLastTokenFromColumn(i);
                return i;
            }
            if ( newScore > alpha) {
                alpha = newScore;
                columnOfBestScore = i;
            }
            Board::getInstance()->changePlayerToMove();
            Board::getInstance()->removeLastTokenFromColumn(i);
        }
    }
    return columnOfBestScore;
}

int Bot::getMinMaxScore(int alpha, int beta) {
    std::cout<<"all counter: "<< Board::getInstance()->getAllMovesCounter()<<std::endl;
//    std::cout<<"getMinMaxScore"<<std::endl;
    if (Board::getInstance()->checkDraw() )
        return 0;
    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(i) && Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove()))
            return (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
    }

    int max = (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
    if (beta > max) {
//        std::cout<<"beta > max"<<std::endl;
        beta = max;
        if (alpha >= beta) return beta;
    }
//    int bestScore = -Board::getInstance()->getNumOfFields();

    for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if ( Board::getInstance()->isColumnFree(i) ) {
            Board::getInstance()->dropTokenToColumn(i, Board::getInstance()->getPlayerToMove());
            Board::getInstance()->changePlayerToMove();
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore >= beta) {
//                std::cout<<"newScore >= beta"<<std::endl;
                Board::getInstance()->changePlayerToMove();
                Board::getInstance()->removeLastTokenFromColumn(i);
                return newScore;
            }
            if ( newScore > alpha) {
//                std::cout<<"newScore > alpha"<<std::endl;
                alpha = newScore;
            }

            Board::getInstance()->changePlayerToMove();
            Board::getInstance()->removeLastTokenFromColumn(i);
        }
    }
    return alpha;
}

void Bot::wait() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.5, 0.6);
    QTime dieTime = QTime::currentTime().addSecs(distribution(generator));
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Bot::resetBot() {
    mode = 0;
}
