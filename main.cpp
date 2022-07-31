#include "mainthread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainthread w;
    w.show();

    return a.exec();
}
