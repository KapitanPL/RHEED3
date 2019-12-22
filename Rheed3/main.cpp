#include "mainwindow.h"

#include <QApplication>
#include <./Globals/globals.h>
#include "rheedCore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<wchar_t>();
    qRegisterMetaType<S_variant>();
    C_globals * pglobals = &C_globals::getInstance();
    if (pglobals)
    {
        pglobals->Load();
        pglobals->setupLogger();
    }
	rheedCore *AppCore = &rheedCore::GetInstance();

    MainWindow w(AppCore);
    w.show();
    return a.exec();
}
