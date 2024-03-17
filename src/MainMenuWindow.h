#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
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
    void on_startButtonPvP_clicked();
    void on_startButtonPvC_clicked();
    void on_startButtonCvC_clicked();
    void on_loadButton_clicked();

private:
    void showErrorMessageBox(const QString& errorMessage);
    void startLoadedGame(GameCustomization customGame);

private:
    Ui::MainMenuWindow *ui;
};

#endif // MAINMENUWINDOW_H
