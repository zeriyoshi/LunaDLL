#include "mainlauncherwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainLauncherWindow w;
    w.loadConfig("Launcher.ini");
    w.show();

    return a.exec();
}
