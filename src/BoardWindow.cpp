#include <QPainter>
#include "BoardWindow.h"
#include "ui_BoardWindow.h"
#include "debug.h"
#include <array>

BoardWindow::BoardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
    connectSignalsAndSlotsForColumnButtons();

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    QBrush image;
    QPen pen;
    int shiftW = (width()-80)/Board::getInstance()->getWidth();
    int shiftH = (height()-150)/Board::getInstance()->getHeight();

    for(int i =0; i<Board::getInstance()->getWidth(); i++) {
        std::vector <QGraphicsEllipseItem*> rectV;
        for(int j = 0; j < Board::getInstance()->getHeight(); j++) {
            QGraphicsEllipseItem *newEllipse = scene->addEllipse( i*shiftW, j*shiftH, 60, 60, pen, QBrush(Qt::white) );
            rectV.push_back(newEllipse);
        }
        qtBoard.push_back(rectV);
    }

    listOfButtons[0] = ui->column0Button;
    listOfButtons[1] = ui->column1Button;
    listOfButtons[2] = ui->column2Button;
    listOfButtons[3] = ui->column3Button;
    listOfButtons[4] = ui->column4Button;
    listOfButtons[5] = ui->column5Button;
    listOfButtons[6] = ui->column6Button;

    for(int i = 0; i < 7; i++) {
        listOfButtons[i]->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    }

    disableButtons();
    resetWindow();

    if (Board::getInstance()->LOAD_TRANSPOSITION_TABLE)
        Board::getInstance()->loadTranspositionTableFromFile();
}

void BoardWindow::connectSignalsAndSlotsForColumnButtons() {
    std::array columnButtons {
        ui->column0Button,
        ui->column1Button,
        ui->column2Button,
        ui->column3Button,
        ui->column4Button,
        ui->column5Button,
        ui->column6Button
    };

    for (int i = 0; i < columnButtons.size(); ++i) {
        connect(columnButtons.at(i), &QAbstractButton::clicked, this, [=] (bool) -> void { onColumnButtonClicked(columnButtons.at(i), i); });
    }
}

BoardWindow::~BoardWindow() {
    delete ui;
}

void BoardWindow::refreshWindow() {

    for (int i = 0; i < Board::getInstance()->getHeight(); i++) {
        for (int j = 0; j < Board::getInstance()->getWidth(); j++) {
            if (Board::getInstance()->getFields()[i][j] == 1) {
                qtBoard[j][i]->setBrush(QBrush(Qt::red));
            } else if (Board::getInstance()->getFields()[i][j] == 2) {
                qtBoard[j][i]->setBrush(QBrush(Qt::blue));
            } else if (Board::getInstance()->getFields()[i][j] == 0) {
                qtBoard[j][i]->setBrush(QBrush(Qt::NoBrush));
            }
        }
    }
}

void BoardWindow::onColumnButtonClicked(QPushButton* columnButton, int columnIndex) {

    switch(gameMode) {
    case 0:
        if (playerTurn(columnIndex) == false) return;
        break;
    case 1:
        if (playerTurn(columnIndex) == false) return;
        auto start = std::chrono::high_resolution_clock::now();
        if (botTurn() == false) return;
        auto end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        break;
    }
    enableNotFullColumns();

    ui->undoButton->setEnabled(true);

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    Board::getInstance()->printTotalHashTime();
    Board::getInstance()->printTotalSearchTime();
}

bool BoardWindow::checkWinOnBoard(int columnIndex) {
    if (Board::getInstance()->checkWin(columnIndex, Board::getInstance()->getLastDroppedRowInColumn(columnIndex), Board::getInstance()->getPlayerToMove())) {
        endOfGame = true;
        std::vector <std::pair<int, int>> winPositions = Board::getInstance()->getWinPositions();
        for (int i = 0; i < winPositions.size(); i++) {
                if (Board::getInstance()->getPlayerToMove() == 1) {
                    qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(139,0,0)));
                    if (Bot::getInstance()->getMode() == 0)
                        ui->resultLabel->setText("Player 1 won!!!");
                    else
                        ui->resultLabel->setText("Player won!!!");
                }
                else {
                    qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(0,0,139)));
                    if (Bot::getInstance()->getMode() == 0)
                        ui->resultLabel->setText("Player 2 won!!!");
                    else
                        ui->resultLabel->setText("Computer won!!!");
                }
        }
        disableButtons();
        return true;
    }
    return false;
}

bool BoardWindow::checkDrawOnBoard() {
    if (Board::getInstance()->checkDraw()) {
        ui->resultLabel->setText("Draw!!!");
        endOfGame = true;
        return true;
    }
    return false;
}

bool BoardWindow::playerTurn(int columnIndex) {
    Board::getInstance()->dropTokenToColumn(columnIndex, Board::getInstance()->getPlayerToMove());
    disableButtons();
    refreshWindow();
    if(checkWinOnBoard(columnIndex) || checkDrawOnBoard())
        return false;
    Board::getInstance()->changePlayerToMove();
    return true;
}

bool BoardWindow::botTurn() {
    int botMove = Bot::getInstance()->botTurn();
    if(checkWinOnBoard(botMove) || checkDrawOnBoard()) {
        if (checkDrawOnBoard()) refreshWindow();
        return false;
    }
    refreshWindow();
    Board::getInstance()->changePlayerToMove();
    return true;
}

void BoardWindow::startCvCGame() {
    ui->undoButton->setEnabled(false);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &BoardWindow::makeBotMove);
    gameTimer->start(100);
}

void BoardWindow::makeBotMove() {
    static bool isBotMoving = false;
    if (!isBotMoving) {
        isBotMoving = true;
        if (botTurn() == false) {
            gameTimer->stop();
            gameTimer->deleteLater();
            isBotMoving = false;
            return;
        }
        changeBot();
        isBotMoving = false;
    }
}

void BoardWindow::stopTimer() {
    QTimer *timer = qobject_cast<QTimer *>(sender());
    if (timer) {
        timer->stop();
        timer->deleteLater();
    }
}

void BoardWindow::changeBot() {
    if(Bot::getInstance()->getMode() == firstBot) Bot::getInstance()->setMode(secondBot);
    else Bot::getInstance()->setMode(firstBot);
}

void BoardWindow::on_exitButton_clicked() {
    if (Board::getInstance()->SAVE_TRANSPOSITION_TABLE)
        Board::getInstance()->saveTranspositionTableToFile();
    this->close();
}

void BoardWindow::disableButtons() {
    for(int i = 0; i < 7; i++) {
        listOfButtons[i]->setEnabled(false);
    }
}

void BoardWindow::enableNotFullColumns() {
    if (endOfGame) return;
    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        if (Board::getInstance()->isColumnFree(i)) listOfButtons[i]->setEnabled(true);
    }
}

void BoardWindow::resetWindow() {
    Board::getInstance()->resetBoard();
    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        listOfButtons[i]->setEnabled(true);
    }
    ui->resultLabel->setText("");
    endOfGame = false;
}

void BoardWindow::on_newGameButton_clicked() {
    resetWindow();
    refreshWindow();
    switch(gameMode) {
    case 1:
        if (!playerStart) {
            botTurn();
        }
        break;
    case 2:
        startCvCGame();
        break;
    }
}

void BoardWindow::on_undoButton_clicked() {
    Board::getInstance()->changePlayerToMove();
    Board::getInstance()->removeLastTokenFromColumn(Board::getInstance()->getLastColumn());
    ui->undoButton->setEnabled(false);
    refreshWindow();
}

void BoardWindow::setGameMode(int gameMode, bool playerStart, int firstBot, int secondBot) {
    this->gameMode = gameMode;
    this->playerStart = playerStart;
    this->firstBot = firstBot;
    this->secondBot = secondBot;
    Bot::getInstance()->setMode(firstBot);

    switch(gameMode) {
    case 0:
        ui->redLabel->setText("Player 1");
        ui->blueLabel->setText("Player 2");
        break;
    case 1:
        if (playerStart) {
            ui->redLabel->setText("Player");
            ui->blueLabel->setText("Bot");
        } else {
            ui->redLabel->setText("Bot");
            ui->blueLabel->setText("Player");
        }
        break;
    case 2:
        ui->redLabel->setText(botList[firstBot]);
        ui->blueLabel->setText(botList[secondBot]);
        disableButtons();
        break;
    }
}