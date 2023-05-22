#include "MainMenuWindow.h"
#include "ui_MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenuWindow)
{
    ui->setupUi(this);
    ui->startButtonPvC->setDisabled(1);
}

MainMenuWindow::~MainMenuWindow()
{
    delete ui;
}

void MainMenuWindow::on_startButtonPvP_clicked()
{
    BoardWindow *bw = new BoardWindow();
    bw->show();
    this->hide();
}


void MainMenuWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

