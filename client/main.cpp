#include "mainwindow.h"
#include "mapping.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    Mapping *map = new Mapping;
    map->start();
    w.show();
    return a.exec();
}
