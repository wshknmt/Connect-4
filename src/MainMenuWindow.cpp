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
    bw->setGameMode(0, true, 0, 0, false);
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
void MainMenuWindow::on_loadButton_clicked() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath.parent_path().parent_path().parent_path();
    targetPath = targetPath / "saves";

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Board from file"),  QString::fromStdString(targetPath.string()),tr("Connect Four files (*.c4)"));
    if (!fileName.isEmpty()) {

        std::ifstream file(fileName.toStdString());
        if (file.is_open()) {

            GameCustomization gameCustomization = Board::getInstance()->loadFile(file);
            if (!gameCustomization.message.empty()) showErrorMessageBox(QString::fromStdString(gameCustomization.message));
            else startLoadedGame(gameCustomization);
            file.close();

        } else {
            std::cerr << "Unable to open file: " << fileName.toStdString() << std::endl;
        }
    }
}

void MainMenuWindow::showErrorMessageBox(const QString& errorMessage) {
    QMessageBox::critical(nullptr, "Error", errorMessage, QMessageBox::Ok);
}

void MainMenuWindow::startLoadedGame(GameCustomization customGame) {

    if (customGame.p1Counter == customGame.p2Counter) Board::getInstance()->setPlayerToMove(1);
    else Board::getInstance()->setPlayerToMove(2);
    BoardWindow *bw = new BoardWindow();

    switch(customGame.gameMode) {
        case 0:
            bw->setGameMode(0, true, 0, 0, true);
            break;
        case 1:
            if (customGame.player1 == 0) {
                bw->setGameMode(1, true, customGame.player2, 0, true);
                if ( !(customGame.p1Counter == customGame.p2Counter) ) {
                    if (bw->botTurn() == false) {
                        bw->show();
                        this->hide();
                        return;
                    }
            } else {
                bw->setGameMode(1, false, customGame.player1, 0, true);
                if (customGame.p1Counter == customGame.p2Counter) {
                    if (bw->botTurn() == false) {
                        bw->show();
                        this->hide();
                        return;
                    }
                }
            }
            break;
        case 2:
            bw->setGameMode(2, true, customGame.player1, customGame.player2, true);
            bw->startCvCGame();
            break;
        }
        bw->refreshWindow();
        bw->show();
        this->hide();
}


