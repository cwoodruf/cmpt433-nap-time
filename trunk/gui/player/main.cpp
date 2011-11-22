#include <QtGui/QApplication>
#include "PlayerWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerWindow w;
    w.showMaximized();

    return a.exec();
}
