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

    ui->undoButton->setEnabled(false);
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
    playerTurnLabel();
    ui->undoButton->setEnabled(true);

    // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    // Board::getInstance()->printTotalHashTime();
    // Board::getInstance()->printTotalSearchTime();
}

void BoardWindow::playerTurnLabel() {
    if (gameMode == 0) {
        if (Board::getInstance()->getPlayerToMove() == 1)
            ui->resultLabel->setText("Player 1 turn");
        else ui->resultLabel->setText("Player 2 turn");
    } else ui->resultLabel->setText("Player turn");
}

bool BoardWindow::checkWinOnBoard(int columnIndex) {
    if (Board::getInstance()->checkWin(columnIndex, Board::getInstance()->getLastDroppedRowInColumn(columnIndex), Board::getInstance()->getPlayerToMove())) {
        endOfGame = true;
        ui->undoButton->setEnabled(false);
        std::vector <std::pair<int, int>> winPositions = Board::getInstance()->getWinPositions();
        for (int i = 0; i < winPositions.size(); i++) {
            if (Board::getInstance()->getPlayerToMove() == 1) {
                qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(139,0,0)));
                QString labelText = ui->redLabel->text();
                ui->resultLabel->setText(labelText + " won !!!");
            }
            else {
                qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(0,0,139)));
                QString labelText = ui->blueLabel->text();
                ui->resultLabel->setText(labelText + " won !!!");
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
    Board::getInstance()->addElementToMovesHistory(columnIndex);
    disableButtons();
    refreshWindow();
    if(checkWinOnBoard(columnIndex) || checkDrawOnBoard())
        return false;
    Board::getInstance()->changePlayerToMove();
    return true;
}

bool BoardWindow::botTurn() {
    botThinkingLabel();
    int botMove = Bot::getInstance()->botTurn();
    Board::getInstance()->addElementToMovesHistory(botMove);
    if(checkWinOnBoard(botMove) || checkDrawOnBoard()) {
        if (checkDrawOnBoard()) refreshWindow();
        return false;
    }
    if (gameMode != 2) playerTurnLabel();
    refreshWindow();
    Board::getInstance()->changePlayerToMove();
    return true;
}

void BoardWindow::botThinkingLabel() {
    if (gameMode == 1) {
        ui->resultLabel->setText("Bot thinking...");
    } else {
        if (Board::getInstance()->getPlayerToMove() == 1) {
            ui->resultLabel->setText("Red Bot thinking...");
        } else {
            ui->resultLabel->setText("Blue bot thinking...");
        }
    }
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
    if (!loaded) Board::getInstance()->resetBoard();
    for (int i = 0; i < Board::MAX_TEST_COLUMN; i++) {
        listOfButtons[i]->setEnabled(true);
    }
    ui->resultLabel->setText("");
    endOfGame = false;
    Board::getInstance()->clearHistorySize();
    ui->undoButton->setEnabled(false);
}

void BoardWindow::on_newGameButton_clicked() {
    loaded = false;
    resetWindow();
    refreshWindow();
    switch(gameMode) {
    case 1:
        if (!playerStart) {
            botTurn();
        }
        break;
    case 2:
        Bot::getInstance()->setMode(firstBot);
        startCvCGame();
        break;
    }
}

void BoardWindow::on_undoButton_clicked() {

    switch(gameMode) {
    case 0:
        Board::getInstance()->changePlayerToMove();
        Board::getInstance()->removeLastTokenFromColumn(Board::getInstance()->popBackMovesHistory());
        break;
    case 1:
        Board::getInstance()->changePlayerToMove();
        Board::getInstance()->removeLastTokenFromColumn(Board::getInstance()->popBackMovesHistory());
        Board::getInstance()->changePlayerToMove();
        Board::getInstance()->removeLastTokenFromColumn(Board::getInstance()->popBackMovesHistory());
        break;
    }

    if (Board::getInstance()->movesHistorySize() == 0) ui->undoButton->setEnabled(false);

    refreshWindow();
}

void BoardWindow::setGameMode(int gameMode, bool playerStart, int firstBot, int secondBot, bool loaded) {
    this->gameMode = gameMode;
    this->playerStart = playerStart;
    this->firstBot = firstBot;
    this->secondBot = secondBot;
    this->loaded = loaded;
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
        QFont currentFont = ui->resultLabel->font();
        currentFont.setPointSize(14);
        ui->resultLabel->setFont(currentFont);
        break;
    }

    if (gameMode != 2) playerTurnLabel();
}
void BoardWindow::on_exportButton_clicked() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath.parent_path().parent_path().parent_path();
    targetPath = targetPath / "saves";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Board to file"),  QString::fromStdString(targetPath.string()),tr("Connect Four files (*.c4)"));
    if (!fileName.isEmpty()) {
        std::ofstream file(fileName.toStdString());
        if (file.is_open()) {
            file<<"MODE: 0 - PvP, 1 - PvC, 2 - CvC, PLAYER: 0 - Human, 1 - Random Bot, 2 - Naive heur. Bot, 3 - MinMax heur. Bot, 4 - Perfect Bot"<<std::endl;
            file<<"MODE,PLAYER1,PLAYER2"<<std::endl;

            switch(gameMode) {
            case 0:
                file<<"0,0,0"<<std::endl;
                break;
            case 1:
                if (playerStart) {
                    file<<"1,0,"<<firstBot<<std::endl;
                } else {
                    file<<"1,"<<firstBot<<",0"<<std::endl;
                }
                break;
            case 2:
                file<<"2,"<<firstBot<<","<<secondBot<<std::endl;
                break;
            }

            for (int i = 0; i < Board::getInstance()->getHeight(); i++) {
                for (int j = 0; j < Board::getInstance()->getWidth(); j++) {
                    file << Board::getInstance()->getFields()[i][j] << ",";
                }
                file<<std::endl;
            }
            file.close();
            std::cout << "Data saved to " << fileName.toStdString() << std::endl;
        } else {
            std::cerr << "Unable to save file: " << fileName.toStdString() << std::endl;
        }
    }
}

