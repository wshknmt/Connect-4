
#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include <QShortcut>
#include <filesystem>
#include "Board.h"
#include "Bot.h"


QT_BEGIN_NAMESPACE
namespace Ui { class BoardWindow; }
QT_END_NAMESPACE

class BoardWindow : public QMainWindow

{
    Q_OBJECT

public:
    BoardWindow(QWidget *parent = nullptr);
    ~BoardWindow();
    void refreshWindow();
    void buttonCLicked();
    void disableButtons();
    void enableNotFullColumns();
    void resetWindow();
    void setGameMode(int gameMode, bool playerStart, int firstBot, int secondBot, bool loaded);
    bool playerTurn(int columnIndex); // return false if end of game
    bool botTurn(); // return false if end of game
    void startCvCGame();
    void changeBot();
    void makeBotMove();
    void stopTimer();

private slots:
    void connectSignalsAndSlotsForColumnButtons();
    void on_exitButton_clicked();
    void on_newGameButton_clicked();
    void on_undoButton_clicked();
    void on_exportButton_clicked();
    void on_changePlayer();

private:
    void onColumnButtonClicked(QPushButton* columnButton, int columnIndex);
    bool checkWinOnBoard(int columnIndex);
    bool checkDrawOnBoard();
    void playerTurnLabel();
    void botThinkingLabel();
    bool perfectBotInUse();

    bool endOfGame = false;
    Ui::BoardWindow *ui;
    QGraphicsScene *scene;
    std::vector<std::vector<QGraphicsEllipseItem*>> qtBoard;
    QPushButton *listOfButtons[9];
    int gameMode; // 0 - PvP, 1 - PvC, 2 - Cvc
    bool playerStart;
    int firstBot, secondBot; // 0 - undefined, 1 - random, 2 - naive heur., 3 - minmax heur., 4 - perfect
    bool loaded; // true - loaded game from file, false - new game
    QString botList[5] = {" ", "Random Bot", "Naive heur. Bot", "MinMax heur. Bot", "Perfect Bot"};
    std::chrono::duration<double> duration;
    QTimer *gameTimer;

};

#endif // BOARDWINDOW_H
