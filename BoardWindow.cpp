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

//void BoardWindow::buttonCLicked() {
//        if (Board::getInstance()->checkWin(4, row, 1)) {
//            std::cout <<"Player won!"<< std::endl;
//        }
//}

void BoardWindow::onColumnButtonClicked(QPushButton* columnButton, int columnIndex)
{
    Board::getInstance()->dropTokenToColumn(columnIndex, Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(columnIndex)) {
        columnButton->setEnabled(false);
    }
    refreshWindow();
//    buttonCLicked();
}


//void BoardWindow::on_column1Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(1,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(1)) {
//        ui->column1Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}

//void BoardWindow::on_column2Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(2,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(2)) {
//        ui->column2Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}

//void BoardWindow::on_column3Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(3,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(3)) {
//        ui->column3Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}

//void BoardWindow::on_column4Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(4,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(4)) {
//        ui->column4Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}

//void BoardWindow::on_column5Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(5,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(5)) {
//        ui->column5Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}

//void BoardWindow::on_column6Button_clicked()
//{
//    Board::getInstance()->dropTokenToColumn(6,Board::getInstance()->getPlayerToMove());
//    Board::getInstance()->print();
//    if (!Board::getInstance()->isColumnFree(6)) {
//        ui->column6Button->setEnabled(false);
//    }
//    refreshWindow();
//    buttonCLicked();
//}
