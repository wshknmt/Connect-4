
#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QImage>
#include "Board.h"


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

private slots:

    void on_column0Button_clicked();

    void on_column6Button_clicked();

    void on_column1Button_clicked();

    void on_column2Button_clicked();

    void on_column3Button_clicked();

    void on_column4Button_clicked();

    void on_column5Button_clicked();

private:
    Ui::BoardWindow *ui;
    QGraphicsScene *scene;
    std::vector<std::vector<QGraphicsEllipseItem*>> qtBoard;


};

#endif // BOARDWINDOW_H
