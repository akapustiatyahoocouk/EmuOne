//
//  emuone-util/PluginManager.cpp
//
//  PluginManager class implementation
//////////
#include "emuone-util/API.hpp"

QSet<QString> PluginManager::_processedDllPaths;
PluginList PluginManager::_loadedPlugins;
PluginList PluginManager::_initialisezPlugins;

//////////
//  Operations
void PluginManager::loadPlugins()
{
    QString startupPath = QCoreApplication::applicationDirPath();
    //  Inspect all DLLs/SOs in the "startupPath"
    QDir dir(startupPath);
    QStringList fileNames = dir.entryList(QDir::Files);
    for (QString fileName : fileNames)
    {   //  DLLs/SOs only
        if (QFileInfo(fileName).isSymLink() || !QLibrary::isLibrary(fileName))
        {
            continue;
        }
        qDebug() << "Library found: " << fileName;
        //  Process each DLL once
        QString dllPath = startupPath + QDir::separator() + fileName;
        if (_processedDllPaths.contains(dllPath))
        {
            continue;
        }
        _processedDllPaths.insert(dllPath);
        //  Is there a "PluginExportProc" there ?
        QLibrary * library = new QLibrary(dllPath);
        if (library->load())
        {
            PluginExportProc pluginExportProc = (PluginExportProc)library->resolve("PluginExportProc");
            if (pluginExportProc == nullptr)
            {   //  No!
                delete library;
                continue;
            }
            //  Get the plugins
            PluginList pluginList;
            (*pluginExportProc)(pluginList);
            if (pluginList.size() == 0)
            {   //  None exported!
                delete library;
                continue;
            }
            //  Record
            qDebug() << "Plugins found in " << fileName;
            _loadedPlugins.append(pluginList);
        }
    }
    //  Initialise what we can
    for (bool keepGoing = true; keepGoing; )
    {
        keepGoing = false;
        for (Plugin * plugin : _loadedPlugins)
        {
            if (!_initialisezPlugins.contains(plugin))
            {
                try
                {
                    plugin->initialise();
                    _initialisezPlugins.append(plugin);
                    keepGoing = true;
                }
                catch (...)
                {   //  OOPS!
                }
            }
        }
    }
}

//  End of emuone-util/PluginManager.cpp
