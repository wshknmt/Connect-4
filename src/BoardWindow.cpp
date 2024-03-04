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

    ui->column0Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column1Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column2Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column3Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column4Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column5Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
    ui->column6Button->setStyleSheet("QPushButton { background-color: rgba(1, 255, 255, 0); }" "QPushButton:hover { background-color: rgba(250, 250, 250, 100); }");
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
    Board::getInstance()->dropTokenToColumn(columnIndex, Board::getInstance()->getPlayerToMove());
    disableButtons();
    refreshWindow();
    if(checkWinOnBoard(columnIndex) || checkDrawOnBoard())
        return;
    Board::getInstance()->changePlayerToMove();

    auto start = std::chrono::high_resolution_clock::now();

    if (Bot::getInstance()->getMode() != 0) {
        int botMove = Bot::getInstance()->botTurn();
        if(checkWinOnBoard(botMove) || checkDrawOnBoard()) {
            if (checkDrawOnBoard()) refreshWindow();
            return;
        }
        refreshWindow();
        enableNotFullColumns();
        Board::getInstance()->changePlayerToMove();
    } else if (Bot::getInstance()->getMode() == 0) {
        enableNotFullColumns();
    }
    Board::getInstance()->print();
    ui->undoButton->setEnabled(true);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
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

void BoardWindow::on_exitButton_clicked() {
    if (Board::getInstance()->SAVE_TRANSPOSITION_TABLE)
        Board::getInstance()->saveTranspositionTableToFile();
    this->close();
}

void BoardWindow::disableButtons() {
    ui->column0Button->setEnabled(false);
    ui->column1Button->setEnabled(false);
    ui->column2Button->setEnabled(false);
    ui->column3Button->setEnabled(false);
    ui->column4Button->setEnabled(false);
    ui->column5Button->setEnabled(false);
    ui->column6Button->setEnabled(false);
}

void BoardWindow::enableNotFullColumns() {
    if (endOfGame) return;
    if (Board::getInstance()->isColumnFree(0)) ui->column0Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(1)) ui->column1Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(2)) ui->column2Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(3)) ui->column3Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(4)) ui->column4Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(5)) ui->column5Button->setEnabled(true);
    if (Board::getInstance()->isColumnFree(6)) ui->column6Button->setEnabled(true);
}

void BoardWindow::resetWindow() {
    Board::getInstance()->resetBoard();
//    Bot::getInstance()->resetBot();
    ui->column0Button->setEnabled(true);
    ui->column1Button->setEnabled(true);
    ui->column2Button->setEnabled(true);
    ui->column3Button->setEnabled(true);
    ui->column4Button->setEnabled(true);
    ui->column5Button->setEnabled(true);
    ui->column6Button->setEnabled(true);
    ui->resultLabel->setText("");
    endOfGame = false;
}

void BoardWindow::on_newGameButton_clicked() {
    resetWindow();
    refreshWindow();
}

void BoardWindow::on_undoButton_clicked() {
    Board::getInstance()->changePlayerToMove();
    Board::getInstance()->removeLastTokenFromColumn(Board::getInstance()->getLastColumn());
    ui->undoButton->setEnabled(false);
    refreshWindow();
}