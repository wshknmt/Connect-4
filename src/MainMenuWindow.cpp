#include "MainMenuWindow.h"
#include "ui_MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenuWindow)
{
    ui->setupUi(this);
}

MainMenuWindow::~MainMenuWindow() {
    delete ui;
}

void MainMenuWindow::on_exitButton_clicked() {
    QApplication::quit();
}

void MainMenuWindow::on_startButtonPvP_clicked() {
    BoardWindow *bw = new BoardWindow();
    bw->setGameMode(0, true, 0, 0);
    bw->show();
    this->hide();
}

void MainMenuWindow::on_startButtonPvC_clicked() {
    ChooseBotWindow *cw = new ChooseBotWindow();
    cw->show();
    this->hide();
}

void MainMenuWindow::on_startButtonCvC_clicked() {
    CustomBotWindow *cw = new CustomBotWindow();
    cw->show();
    this->hide();
}