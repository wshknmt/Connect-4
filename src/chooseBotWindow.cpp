#include "ChooseBotWindow.h"
#include "ui_ChooseBotWindow.h"

ChooseBotWindow::ChooseBotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseBotWindow)
{
    ui->setupUi(this);
}

ChooseBotWindow::~ChooseBotWindow() {
    delete ui;
}

void ChooseBotWindow::openBoardWindow(int gameMode) {
    BoardWindow *bw = new BoardWindow();
    Bot::getInstance()->setMode(gameMode);
    bw->show();
    this->hide();
}

void ChooseBotWindow::on_startButtonPvCRand_clicked() {
    openBoardWindow(1);
}


void ChooseBotWindow::on_startButtonPvCMinMax_clicked() {
    openBoardWindow(2);
}


void ChooseBotWindow::on_startButtonPvCSingleHeuristic_clicked() {
    openBoardWindow(3);
}


void ChooseBotWindow::on_startButtonPvCHeuristic_clicked() {
    openBoardWindow(4);
}

