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
    if (index == 4) {
        ui->secondBotCombo->removeItem(4);
        ui->secondBotCombo->setCurrentIndex(0);
    }
    else if (ui->secondBotCombo->count() == 4) {
        ui->secondBotCombo->addItem("Perfect Bot");
    }
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
    bw->setGameMode(2, true, ui->firstBotCombo->currentIndex(), ui->secondBotCombo->currentIndex());
    bw->show();
    this->hide();
    bw->startCvCGame();
}