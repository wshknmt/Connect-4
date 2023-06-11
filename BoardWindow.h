
#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPushButton>
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
    void resetWindow();

private slots:
    void connectSignalsAndSlotsForColumnButtons();

    void on_exitButton_clicked();

private:
    void onColumnButtonClicked(QPushButton* columnButton, int columnIndex);
    bool checkWinOnBoard(int columnIndex);

    Ui::BoardWindow *ui;
    QGraphicsScene *scene;
    std::vector<std::vector<QGraphicsEllipseItem*>> qtBoard;

};

#endif // BOARDWINDOW_H
