#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QMainWindow>
#include "CustomBotWindow.h"
#include "ChooseBotWindow.h"

namespace Ui {
class MainMenuWindow;
}

class MainMenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenuWindow(QWidget *parent = nullptr);
    ~MainMenuWindow();

private slots:

    void on_exitButton_clicked();
    void openBoardWindow(int gameMode);
    void on_startButtonPvP_clicked();
    void on_startButtonPvC_clicked();
    void on_startButtonCvC_clicked();

private:
    Ui::MainMenuWindow *ui;
};

#endif // MAINMENUWINDOW_H
