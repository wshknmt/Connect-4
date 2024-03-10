#ifndef CUSTOMBOTWINDOW_H
#define CUSTOMBOTWINDOW_H

#include <QMainWindow>
#include "BoardWindow.h"

namespace Ui {
class CustomBotWindow;
}

class CustomBotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomBotWindow(QWidget *parent = nullptr);
    ~CustomBotWindow();

private slots:
    void on_firstBotCombo_currentIndexChanged(int index);
    void on_secondBotCombo_currentIndexChanged(int index);

private:
    Ui::CustomBotWindow *ui;
};

#endif // CUSTOMBOTWINDOW_H
