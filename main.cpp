#include "mainwindow.h"
#include "audioengine.h"
#include "configmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioEngineInstance();
    ConfigManagerInstance();
    MainWindow w;
    w.show();
    int e = a.exec();
    return e;
}
