#include "mainwindow.h"
#include "audioengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioEngineInstance();
    MainWindow w;
    w.show();
    int e = a.exec();
    return e;
}
