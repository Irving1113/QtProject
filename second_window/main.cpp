#include <QApplication>
#include "qbstopwatch.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QBStopWatch w;
    w.InitData();
    w.show();
    return a.exec();
}
