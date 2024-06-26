#include "ChooseBotWindow.h"
#include "ui_ChooseBotWindow.h"

ChooseBotWindow::ChooseBotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseBotWindow)
{
    ui->setupUi(this);
    ui->frame->lower();
}

ChooseBotWindow::~ChooseBotWindow() {
    delete ui;
}

void ChooseBotWindow::openBoardWindow(int bot) {
    BoardWindow *bw = new BoardWindow();
    bw->setGameMode(1, ui->playerRadio->isChecked(), bot, 0, false);
    bw->show();
    this->hide();
    if (!ui->playerRadio->isChecked()) {
        auto start = std::chrono::high_resolution_clock::now();
        bw->botTurn();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    }
}

void ChooseBotWindow::on_startButtonPvCRand_clicked() {
    openBoardWindow(1);
}

void ChooseBotWindow::on_startButtonPvCSingleHeuristic_clicked() {
    openBoardWindow(2);
}

void ChooseBotWindow::on_startButtonPvCHeuristic_clicked() {
    openBoardWindow(3);
}

void ChooseBotWindow::on_startButtonPvCMinMax_clicked() {
    openBoardWindow(4);
}

