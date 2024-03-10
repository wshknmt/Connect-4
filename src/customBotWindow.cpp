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

}


void CustomBotWindow::on_secondBotCombo_currentIndexChanged(int index) {

}

