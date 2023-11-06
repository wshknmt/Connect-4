#include "Bot.h"

Bot* Bot::pInstance = nullptr;

int MAX_TEST_COLUMN = 4;

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

void Bot::setMode(int new_mode) {
    mode = new_mode;
}

int Bot::getMode() {
    return mode;
}

void Bot::play(int column) {
    Board::getInstance()->dropTokenToColumn(column, Board::getInstance()->getPlayerToMove());
    Board::getInstance()->changePlayerToMove();
}
void Bot::undoPlay(int column) {
    Board::getInstance()->changePlayerToMove();
    Board::getInstance()->removeLastTokenFromColumn(column);
}

int Bot::getMove() {
    // std::cout<<"mode: "<<getMode()<<std::endl<<std::endl;
    switch(getMode()) {
    case 1:
        return getRandomMove();
        break;
    case 2:
        return getMinMaxMove( -Board::getInstance()->getNumOfFields() / 2, Board::getInstance()->getNumOfFields() / 2);
        // return getMinMaxMove( -1, 1);
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

    for (int i = 0; i < MAX_TEST_COLUMN; i++) {
    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(i) && Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove()))
            return i;
    }

    int columnOfBestScore = -1;

    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
    for (int i = 0; i < MAX_TEST_COLUMN; i++) {
        if ( Board::getInstance()->isColumnFree(i) ) {
            play(i);
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore > alpha) {
                alpha = newScore;
                columnOfBestScore = i;
            }
            undoPlay(i);

            std::cout<<"column: "<<i<<", score: "<<newScore<<std::endl;
            std::cout<<"checked: "<<100.0*(i+1)/MAX_TEST_COLUMN<<" % column"<<std::endl;
            std::cout<<"recursiveLevel: "<<recursiveLevel<<std::endl;
        }
            
    }
    return columnOfBestScore;
}

int Bot::getMinMaxScore(int alpha, int beta) {
    recursiveLevel++;
    // int x = Board::getInstance()->getNumOfFields();
    // if (recursiveLevel >= x/4.0) {
    //     recursiveLevel--;
    //     return alpha;
    // }
    if (Board::getInstance()->getAllMovesCounter() % 1000000 == 0) 
        std::cout<<"all counter: "<< Board::getInstance()->getAllMovesCounter()<<std::endl;
    if (Board::getInstance()->checkDraw() ) {
        recursiveLevel--;
        return 0;
    }
        

    for (int i = 0; i < MAX_TEST_COLUMN; i++) {
    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(i) && Board::getInstance()->checkWin(i, Board::getInstance()->getFreeRowInColumn(i), Board::getInstance()->getPlayerToMove())) {
            recursiveLevel--;
            return (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 4;
        }
            
    }

    int max = (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
    if (beta > max) {
        beta = max;
        if (alpha >= beta) {
            recursiveLevel--;
            return beta;
        } 
    }

    for (int i = 0; i < MAX_TEST_COLUMN; i++) {
    //for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if ( Board::getInstance()->isColumnFree(i) ) {
            play(i);
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore >= beta) {
                undoPlay(i);
                recursiveLevel--;
                return newScore;
            }
            if ( newScore > alpha) alpha = newScore;
            
            undoPlay(i);
        }
    }
    recursiveLevel--;
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
