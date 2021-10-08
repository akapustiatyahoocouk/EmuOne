//
//  emuone/Main.cpp
//
//  EmuOne entry point
//
//////////
#include "emuone/API.hpp"

//////////
//  EmuOne entry point
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    util::PluginManager::loadPlugins();

    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}

//  End of emuone/Main.cpp
