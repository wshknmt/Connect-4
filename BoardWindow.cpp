#include <QPainter>
#include "BoardWindow.h"
#include "ui_BoardWindow.h"
#include "debug.h"

BoardWindow::BoardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BoardWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    QBrush image;
    QPen pen;
    int shiftW = (width()-80)/Board::getInstance()->getWidth();
    int shiftH = (height()-150)/Board::getInstance()->getHeight();

    for(int i =0; i<Board::getInstance()->getWidth(); i++) {
        std::vector <QGraphicsEllipseItem*> rectV;
        for(int j = 0; j < Board::getInstance()->getHeight(); j++) {
//            QGraphicsRectItem *newRect;
            QGraphicsEllipseItem *newEllipse = scene->addEllipse( i*shiftW, j*shiftH, 60, 60, pen, QBrush(Qt::white) );
//            newRect = scene->addRect(i*shift, j*shift, shift, shift);
//            FieldType type = Board::getInstance()->getFields()[j][i].getType();
//            if(type == FieldType::WALL) image.setTextureImage(wall);
//            else if(type == FieldType::EGG) image.setTextureImage(egg);
//            else if(type == FieldType::EMPTY) image.setTextureImage(grass);
//            newRect->setBrush(image);
            rectV.push_back(newEllipse);
        }
        qtBoard.push_back(rectV);
    }

//    Board::getInstance()->setField(0,4,2);

    Board::getInstance()->print();

//    Board::getInstance()->dropTokenToColumn(1,1);
//    Board::getInstance()->dropTokenToColumn(2,2);
//    Board::getInstance()->dropTokenToColumn(2,1);
//    Board::getInstance()->dropTokenToColumn(1,2);
//    Board::getInstance()->dropTokenToColumn(3,1);
//    Board::getInstance()->dropTokenToColumn(3,2);
//    Board::getInstance()->dropTokenToColumn(3,1);
//    Board::getInstance()->dropTokenToColumn(3,2);
//    Board::getInstance()->dropTokenToColumn(4,1);
//    Board::getInstance()->dropTokenToColumn(4,2);
//    Board::getInstance()->dropTokenToColumn(4,1);
//    Board::getInstance()->dropTokenToColumn(5,2);
//    int row = Board::getInstance()->dropTokenToColumn(4,1);
//    Board::getInstance()->print();
//    if (Board::getInstance()->checkWin(4, row, 1)) {
//        std::cout <<"Player won!"<< std::endl;
//    }

//    QBrush tb(Qt::transparent); // Transparent brush, solid pattern
//    ui->column0Button->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb)); // Set every color roles to the transparent brush

//    ui->column0Button->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0); }");
    ui->column0Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column1Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column2Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column3Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column4Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column5Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->column6Button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");



//    painter = new QPainter(this);
//    QRectF rectangle(10.0, 20.0, 80.0, 60.0);

//    painter->drawRect(rectangle);
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::refreshWindow() {

        for (int i = 0; i < Board::getInstance()->getHeight(); ++i) {
            for (int j = 0; j < Board::getInstance()->getWidth(); ++j) {
                if (Board::getInstance()->getFields()[i][j] == 1) {
                    qtBoard[i][j]->setBrush(QBrush(Qt::red));
                } else if (Board::getInstance()->getFields()[i][j] == 2) {
                    qtBoard[i][j]->setBrush(QBrush(Qt::blue));
                }
            }
        }
}


void BoardWindow::on_column0Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(0,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(0)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column1Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(1,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(1)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column2Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(2,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(2)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column3Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(3,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(3)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column4Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(4,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(4)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column5Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(5,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(5)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}


void BoardWindow::on_column6Button_clicked()
{
    Board::getInstance()->dropTokenToColumn(6,Board::getInstance()->getPlayerToMove());
    Board::getInstance()->print();
    if (!Board::getInstance()->isColumnFree(6)) {
        ui->column0Button->setEnabled(false);
    }
    refreshWindow();
}

