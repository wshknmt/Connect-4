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
    std::pair<int, int> result;
    switch(getMode()) {
    case 1:
        return getRandomMove();
        break;
    case 2:
        return getHeuristicMove();
        break;
    case 3:
        result = getHeuristicMinMaxMove(6, true);
        return result.second;
        break;
    case 4:
        result = getMinMaxMove( -Board::getInstance()->getNumOfFields() / 2, Board::getInstance()->getNumOfFields() / 2);
        return result.second;
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

std::pair<int, int> Bot::getMinMaxMove(int alpha, int beta) {
    searchDepth++;
    // if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
    //     PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
    //     int score = Board::getInstance()->getScoreFromMap(curHash);
    //     if ( score != 99999) {
    //         searchDepth--;
    //         return std::make_pair(score, -1);
    //     }
    // }
    if (Board::getInstance()->getAllMovesCounter() % 10000000 == 0)
        std::cout<<"all counter: "<< Board::getInstance()->getAllMovesCounter()<<std::endl;
    if (Board::getInstance()->checkDraw() ) {
        searchDepth--;
        return std::make_pair(0, -2);
    }

    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        if( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) && Board::getInstance()->checkWin(Board::getInstance()->getColumnOrder(i), Board::getInstance()->getFreeRowInColumn(Board::getInstance()->getColumnOrder(i)), Board::getInstance()->getPlayerToMove())) {
            searchDepth--;
            return std::make_pair((Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2, Board::getInstance()->getColumnOrder(i));
        }
    }

    int max = (Board::getInstance()->getNumOfFields() + 1 - Board::getInstance()->getMovesCounter()) / 2;
    if (beta > max) {
        beta = max;
        if (alpha >= beta) {
            searchDepth--;
            return std::make_pair(beta, -1);
        }
    }

    int columnOfBestScore = -1;

    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        if ( Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i)) ) {
            play(Board::getInstance()->getColumnOrder(i));
            // if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
            //     PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
            //     int score = Board::getInstance()->getScoreFromMap(curHash);
            //     if ( score != 99999) {
            //         undoPlay(Board::getInstance()->getColumnOrder(i));
            //         searchDepth--;
            //         return std::make_pair(score, Board::getInstance()->getColumnOrder(i));
            //     }
            // }
            std::pair<int, int> newScoreAndColumn = getMinMaxMove(-beta, -alpha);
            newScoreAndColumn.first *= -1;
            if ( newScoreAndColumn.first >= beta) {
                // if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
                //     PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
                //     if (!Board::getInstance()->checkHashInMap(curHash))
                //         Board::getInstance()->addHashToMap(curHash, newScoreAndColumn.first);
                // }
                undoPlay(Board::getInstance()->getColumnOrder(i));
                if (searchDepth == 1) {
                    std::cout<<"column: "<<Board::getInstance()->getColumnOrder(i)<<", score: "<<newScoreAndColumn.first<<std::endl;
                    std::cout<<"checked: "<<100.0*(i+1)/Board::MAX_TEST_COLUMN<<" % column"<<std::endl;
                }
                searchDepth--;
                return std::make_pair(newScoreAndColumn.first, Board::getInstance()->getColumnOrder(i));

            }
            if ( newScoreAndColumn.first > alpha) {
                alpha = newScoreAndColumn.first;
                columnOfBestScore = Board::getInstance()->getColumnOrder(i);
            }

            undoPlay(Board::getInstance()->getColumnOrder(i));
            if (searchDepth == 1) {
                std::cout<<"column: "<<Board::getInstance()->getColumnOrder(i)<<", score: "<<alpha<<std::endl;
                std::cout<<"checked: "<<100.0*(i+1)/Board::MAX_TEST_COLUMN<<" % column"<<std::endl;
            }

        }
    }
    // if (Board::getInstance()->getMovesCounter() <= MAX_TOKENS_TO_HASH_IN_TABLE) {
    //     PairInt64 curHash = Board::getInstance()->hashCurrentPosition();
    //     if (!Board::getInstance()->checkHashInMap(curHash))
    //         Board::getInstance()->addHashToMap(curHash, alpha);
    // }
    searchDepth--;
    return std::make_pair(alpha, columnOfBestScore);
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
            playerScore = playerScore * 2 / 3;
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

std::pair<int, int> Bot::getHeuristicMinMaxMove(int depth, bool maximizingPlayer) {
    if (depth == 0 || Board::getInstance()->checkDraw()) {
        return evaluateHeuristicMinMax();
    }

    if (maximizingPlayer) {
        int bestValue = INT_MIN;
        int bestIndex = -1;
        for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
            if (Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i))) {
                play(Board::getInstance()->getColumnOrder(i));

                std::pair<int, int> value;
                if (Board::getInstance()->checkWin(Board::getInstance()->getColumnOrder(i), Board::getInstance()->getLastDroppedRowInColumn(Board::getInstance()->getColumnOrder(i)), (Board::getInstance()->getPlayerToMove() % 2 + 1))) {
                    value.first = Board::getInstance()->getWeight(4);
                    value.second = Board::getInstance()->getColumnOrder(i);
                } else {
                    value = getHeuristicMinMaxMove(depth - 1, false);
                }
                undoPlay(Board::getInstance()->getColumnOrder(i));

                if (value.first > bestValue) {
                    bestValue = value.first;
                    bestIndex = Board::getInstance()->getColumnOrder(i);
                }
            }
        }
        return std::make_pair(bestValue, bestIndex);
    } else {
        int bestValue = INT_MAX;
        int bestIndex = -1;
        for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
            if (Board::getInstance()->isColumnFree(Board::getInstance()->getColumnOrder(i))) {
                play(Board::getInstance()->getColumnOrder(i));

                std::pair<int, int> value;
                if (Board::getInstance()->checkWin(Board::getInstance()->getColumnOrder(i), Board::getInstance()->getLastDroppedRowInColumn(Board::getInstance()->getColumnOrder(i)), (Board::getInstance()->getPlayerToMove() % 2 + 1))) {
                    value.first = -Board::getInstance()->getWeight(4);
                    value.second = Board::getInstance()->getColumnOrder(i);
                } else {
                    value = getHeuristicMinMaxMove(depth - 1, true);
                }
                undoPlay(Board::getInstance()->getColumnOrder(i));
                if (value.first < bestValue) {
                    bestValue = value.first;
                    bestIndex = Board::getInstance()->getColumnOrder(i);
                }
            }
        }
        return std::make_pair(bestValue, bestIndex);
    }
}

std::pair<int, int> Bot::evaluateHeuristicMinMax() {
    int opponentScore = Board::getInstance()->getPointResult(Board::getInstance()->getPlayerToMove());
    int playerScore =   Board::getInstance()->getPointResult(Board::getInstance()->getNextPlayerToMove());
    // opponentScore = playerScore / 3;
    return std::make_pair(playerScore - opponentScore, -1);
}

