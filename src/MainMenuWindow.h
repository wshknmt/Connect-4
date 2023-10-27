#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QMainWindow>
#include "BoardWindow.h"

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
    void on_startButtonPvP_clicked();

    void on_exitButton_clicked();

    void on_startButtonPvCRand_clicked();

    void on_startButtonPvCMinMax_clicked();

private:
    Ui::MainMenuWindow *ui;
};

#endif // MAINMENUWINDOW_H
