#include "mainwindow.h"

#include <QApplication>
#include <./Globals/globals.h>
#include "rheedCore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    C_globals * pglobals = &C_globals::getInstance();
    if (pglobals)
    {
        pglobals->Load();
        pglobals->setupLogger();
    }
	rheedCore AppCore;
    MainWindow w;
    w.show();
    return a.exec();
}