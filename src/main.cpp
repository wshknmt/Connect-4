
#include "BoardWindow.h"
#include "MainMenuWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainMenuWindow m;
    m.show();
    return a.exec();
}
