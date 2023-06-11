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

    Board::getInstance()->print();

    ui->column0Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column1Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column2Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column3Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column4Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column5Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
    ui->column6Button->setStyleSheet("background-color: rgba(1, 255, 255, 50);");
}

void BoardWindow::connectSignalsAndSlotsForColumnButtons()
{
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

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::refreshWindow() {

        for (int i = 0; i < Board::getInstance()->getHeight(); i++) {
            for (int j = 0; j < Board::getInstance()->getWidth(); j++) {
                if (Board::getInstance()->getFields()[i][j] == 1) {
                    qtBoard[j][i]->setBrush(QBrush(Qt::red));
                } else if (Board::getInstance()->getFields()[i][j] == 2) {
                    qtBoard[j][i]->setBrush(QBrush(Qt::blue));
                }
            }
        }
}

void BoardWindow::onColumnButtonClicked(QPushButton* columnButton, int columnIndex)
{
    Board::getInstance()->dropTokenToColumn(columnIndex, Board::getInstance()->getPlayerToMove());
    if (!Board::getInstance()->isColumnFree(columnIndex)) {
        columnButton->setEnabled(false);
    }
    refreshWindow();
    if(checkWinOnBoard(columnIndex))
        return;
    Board::getInstance()->changePlayerToMove();

    checkWinOnBoard(Bot::getInstance()->botTurn());
    refreshWindow();
    Board::getInstance()->changePlayerToMove();
    Board::getInstance()->print();



}

bool BoardWindow::checkWinOnBoard(int columnIndex) {
    if (Board::getInstance()->checkWin(columnIndex, Board::getInstance()->getLastDroppedRowInColumn(columnIndex), Board::getInstance()->getPlayerToMove())) {
        std::vector <std::pair<int, int>> winPositions = Board::getInstance()->getWinPositions();
        for (int i = 0; i < winPositions.size(); i++) {
                if (Board::getInstance()->getPlayerToMove() == 1) {
                    qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(139,0,0)));
                    if (Board::getInstance()->getBotMode() == 0)
                        ui->resultLabel->setText("Player 1 won!!!");
                    else
                        ui->resultLabel->setText("Player won!!!");
                }
                else {
                    qtBoard[winPositions[i].first][winPositions[i].second]->setBrush(QBrush(QColor(0,0,128)));
                    if (Board::getInstance()->getBotMode() == 0)
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


void BoardWindow::on_exitButton_clicked() {
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

void BoardWindow::resetWindow() {
    ui->column0Button->setEnabled(true);
    ui->column1Button->setEnabled(true);
    ui->column2Button->setEnabled(true);
    ui->column3Button->setEnabled(true);
    ui->column4Button->setEnabled(true);
    ui->column5Button->setEnabled(true);
    ui->column6Button->setEnabled(true);
}

