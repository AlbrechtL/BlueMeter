#include "mainwindow.h"
#include "debug_output.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Handle debug output
    DebugOutput::init();
    DebugOutput::setMainWindow(&w);

    w.show();
    return a.exec();
}
