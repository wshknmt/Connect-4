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
    case 3:
        return getHeuristicMove();
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

    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) && Board::getInstance()->checkWin(Board::getInstance()->getColumnOrder(i), Board::getInstance()->getFreeRowInColumn(Board::getInstance()->getColumnOrder(i)), Board::getInstance()->getPlayerToMove()))
            return Board::getInstance()->getColumnOrder(i);
    }

    int columnOfBestScore = -1;

    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        if ( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) ) {
            play(Board::getInstance()->getColumnOrder(i));
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore > alpha) {
                alpha = newScore;
                columnOfBestScore = Board::getInstance()->getColumnOrder(i);
            }
            undoPlay(Board::getInstance()->getColumnOrder(i));

            std::cout<<"column: "<<Board::getInstance()->getColumnOrder(i)<<", score: "<<newScore<<std::endl;
            std::cout<<"checked: "<<100.0*(i+1)/Board::MAX_TEST_COLUMN<<" % column"<<std::endl;
            std::cout<<"recursiveLevel: "<<recursiveLevel<<std::endl;
        }

    }
    return columnOfBestScore;
}

int Bot::getMinMaxScore(int alpha, int beta) {
    if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
        PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
        int score = Board::getInstance()->getScoreFromMap(curHash);
        if ( score != 99999) {
            return score;
        }
    }
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


    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
    // for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) && Board::getInstance()->checkWin(Board::getInstance()->getColumnOrder(i), Board::getInstance()->getFreeRowInColumn(Board::getInstance()->getColumnOrder(i)), Board::getInstance()->getPlayerToMove())) {
            recursiveLevel--;
            return (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
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

    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
    //for (int i = 0; i < Board::getInstance()->getWidth(); i++) {
        if ( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) ) {
            play(Board::getInstance()->getColumnOrder(i));
            if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
                PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
                int score = Board::getInstance()->getScoreFromMap(curHash);
                if ( score != 99999) {
                    undoPlay(Board::getInstance()->getColumnOrder(i));
                    recursiveLevel--;
                    return score;
                }
            }
            int newScore = -getMinMaxScore(-beta, -alpha);
            if ( newScore >= beta) {
                if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
                    PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
                    if (!Board::getInstance()->checkHashInMap(curHash))
                        Board::getInstance()->addHashToMap(curHash, newScore);
                }
                undoPlay(Board::getInstance()->getColumnOrder(i));
                recursiveLevel--;
                return newScore;
            }
            if ( newScore > alpha) alpha = newScore;

            undoPlay(Board::getInstance()->getColumnOrder(i));
        }
    }
    if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
        PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
        if (!Board::getInstance()->checkHashInMap(curHash))
            Board::getInstance()->addHashToMap(curHash, alpha);
    }
    recursiveLevel--;
    return alpha;
}

void Bot::wait() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.5, 0.6);
    QTime dieTime = QTime::currentTime().addSecs(distribution(generator));
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Bot::resetBot() {
    mode = 0;
}

int Bot::getHeuristicMove() {
    int bestScore = INT_MIN;
    int bestIndex = 0;
    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        if (Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i))) {
            play(Board::getInstance()->getColumnOrder(i));

            int opponentScore = Board::getInstance()->getPointResult(Board::getInstance()->getPlayerToMove());
            Board::getInstance()->changePlayerToMove();
            int playerScore = Board::getInstance()->getPointResult(Board::getInstance()->getPlayerToMove());
            Board::getInstance()->changePlayerToMove();

            int score = playerScore - opponentScore;
            if (score > bestScore) {
                bestScore = score;
                bestIndex = i;
            }
            undoPlay(Board::getInstance()->getColumnOrder(i));
            std::cout<<"column num: "<< Board::getInstance()->getColumnOrder(i) << " Pscore: "<<playerScore<<" OScore " <<opponentScore<<" finScore: "<<score<<std::endl;
        }
    }

    return Board::getInstance()->getColumnOrder(bestIndex);
}

