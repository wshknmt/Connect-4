#include "MainMenuWindow.h"
#include "ui_MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenuWindow)
{
    ui->setupUi(this);
}

MainMenuWindow::~MainMenuWindow()
{
    delete ui;
}

void MainMenuWindow::on_startButtonPvP_clicked()
{
    BoardWindow *bw = new BoardWindow();
    Board::getInstance()->setBotMode(0);
    bw->show();
    this->hide();
}


void MainMenuWindow::on_exitButton_clicked()
{
    QApplication::quit();
}


void MainMenuWindow::on_startButtonPvCRand_clicked()
{
    BoardWindow *bw = new BoardWindow();
    Board::getInstance()->setBotMode(1);
    bw->show();
    this->hide();
}


void MainMenuWindow::on_startButtonPvCMinMax_clicked()
{
    BoardWindow *bw = new BoardWindow();
    Board::getInstance()->setBotMode(2);
    bw->show();
    this->hide();
}

