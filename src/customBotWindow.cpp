#include "CustomBotWindow.h"
#include "ui_CustomBotWindow.h"

CustomBotWindow::CustomBotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomBotWindow) {
    ui->setupUi(this);
}

CustomBotWindow::~CustomBotWindow() {
    delete ui;
}

void CustomBotWindow::on_firstBotCombo_currentIndexChanged(int index) {
    if (index == 0) {
        ui->secondBotCombo->setEnabled(false);
        ui->secondBotCombo->setCurrentIndex(0);
    }
    else {
        ui->secondBotCombo->setEnabled(true);
    }
}

void CustomBotWindow::on_secondBotCombo_currentIndexChanged(int index) {
    if (index != 0 && ui->firstBotCombo->currentIndex() != 0) ui->startButton->setEnabled(true);
    else ui->startButton->setEnabled(false);
}

void CustomBotWindow::on_startButton_clicked() {
    BoardWindow *bw = new BoardWindow();
    bw->setGameMode(2, true, ui->firstBotCombo->currentIndex(), ui->secondBotCombo->currentIndex(), false);
    bw->show();
    this->hide();
    bw->startCvCGame();
}