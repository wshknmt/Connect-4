#ifndef CHOOSEBOTWINDOW_H
#define CHOOSEBOTWINDOW_H

#include <QMainWindow>
#include "BoardWindow.h"

namespace Ui {
class ChooseBotWindow;
}

class ChooseBotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseBotWindow(QWidget *parent = nullptr);
    ~ChooseBotWindow();

private slots:

    void openBoardWindow(int gameMode);
    void on_startButtonPvCRand_clicked();
    void on_startButtonPvCMinMax_clicked();
    void on_startButtonPvCSingleHeuristic_clicked();
    void on_startButtonPvCHeuristic_clicked();

private:
    Ui::ChooseBotWindow *ui;
};

#endif // CHOOSEBOTWINDOW_H
