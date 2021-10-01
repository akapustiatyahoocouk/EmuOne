#include "emuone-util/API.hpp"
#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PluginManager::loadPlugins();

    MainWindow w;
    w.show();
    return a.exec();
}
