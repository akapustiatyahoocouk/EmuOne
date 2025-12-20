//
//  emuone-util/ComponentManager.cpp - emuone::util::ComponentManager class implementation
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-util/API.hpp"
using namespace emuone::util;

struct ComponentManager::_Impl
{
    using Registry = QMap<QString, IComponent*>;

    Mutex       guard;
    Registry    registry;
    Components  initializedComponents;  //  TODO try to kill off
};

namespace
{
    QString iniFileName()
    {
        QDir home = QDir::home();
        return home.filePath(".emuone");
    }
}

//////////
//  Operations
Components ComponentManager::components()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    auto values = impl->registry.values();
    return Components(values.cbegin(), values.cend());
}

bool ComponentManager::registerComponent(IComponent * component)
{
    Q_ASSERT(component != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    QString key = component->mnemonic() + "|" + util::toString(component->version());
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return component == registered;
    }
    impl->registry[key] = component;
    return true;
}

bool ComponentManager::unregisterComponent(
        IComponent * component
    )
{
    Q_ASSERT(component != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    auto key = component->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (component == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

IComponent * ComponentManager::findComponent(const QString & mnemonic, const QVersionNumber & version)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QString key = mnemonic + "|" + toString(version);
    return impl->registry.contains(key) ? impl->registry[key] : nullptr;
}

IComponent * ComponentManager::findComponent(const QString & mnemonic)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    IComponent * result = nullptr;
    for (IComponent * component : impl->registry.values())
    {
        if (component->mnemonic() == mnemonic)
        {
            if (result == nullptr ||
                component->version() > result->version())
            {
                result = component;
            }
        }
    }
    return result;
}

void ComponentManager::discoverComponents()
{
    QString startupDirectory = QCoreApplication::applicationDirPath();
    QString exeFile = QCoreApplication::applicationFilePath();
    //  Discover DLLs/SOs and load Components from them
    for (const auto & dllInfo : QDir(startupDirectory).entryInfoList())
    {
        if (dllInfo.isFile() && !dllInfo.isSymbolicLink() &&
            dllInfo.baseName().startsWith("emuone-") &&
            dllInfo.absoluteFilePath() != exeFile)
        {
            _loadLibrary(dllInfo.absoluteFilePath());
        }
    }
}

void ComponentManager::initializeComponents()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    for (bool keepGoing = true; keepGoing; )
    {
        keepGoing = false;
        for (auto component : impl->registry.values())
        {
            if (!impl->initializedComponents.contains(component))
            {   //  Try this one!
                try
                {
                    component->iniialize(); //  may throw
                    impl->initializedComponents.insert(component);
                    keepGoing = true;
                }
                catch (const Exception & ex)
                {   //  OOPS! Log, but suppress
                    qCritical() << ex;
                }
                catch (const Error & ex)
                {   //  OOPS! Log, but suppress
                    qCritical() << ex;
                }
                catch (...)
                {   //  OOPS! Suppress, though
                }
            }
        }

    }
}

void ComponentManager::deinitializeComponents()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    for (auto component : std::as_const(impl->initializedComponents))
    {
        try
        {   //  Be defensive - cleanup as many as possible
            component->deiniialize();
        }
        catch (const Exception & ex)
        {   //  OOPS! Log, but suppress
            qCritical() << ex;
        }
        catch (const Error & ex)
        {   //  OOPS! Log, but suppress
            qCritical() << ex;
        }
        catch (...)
        {   //  OOPS! Suppress, though
        }
    }
    impl->initializedComponents.clear();
}

void ComponentManager::loadComponentSettings()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QFile iniFile(iniFileName());
    if (iniFile.open(QIODevice::ReadOnly))
    {
        IComponent * currentComponent = nullptr;
        QTextStream iniStream(&iniFile);
        while (!iniStream.atEnd())
        {
            QString line = iniStream.readLine();
            //  Section header ?
            line  = line .trimmed();
            if (line.startsWith("[") && line.endsWith("]"))
            {
                int separatorIndex = static_cast<int>(line.indexOf(':'));
                if (separatorIndex == -1)
                {
                    currentComponent = nullptr;
                    continue;
                }
                try
                {
                    QString componentMnemonic = line.mid(1, separatorIndex - 1);
                    QVersionNumber componentVersion(fromString<QVersionNumber>(line.mid(separatorIndex + 1, line.length() - separatorIndex - 2)));
                    currentComponent = findComponent(componentMnemonic, componentVersion);
                    continue;
                }
                catch (const ParseException & ex)
                {
                    qCritical() << ex;
                    currentComponent = nullptr;
                    continue;
                }
            }
            //  <name> = <value>?
            int eqIndex = static_cast<int>(line.indexOf('='));
            if (eqIndex == -1 || currentComponent == nullptr)
            {
                continue;
            }
            QString settingMnemonic = line.mid(0, eqIndex).trimmed();
            QString settingValueString(line.mid(eqIndex + 1).trimmed());
            if (AbstractSetting * setting = currentComponent->settings()->findSetting(settingMnemonic))
            {
                setting->setValueString(settingValueString);
            }
        }
    }
    else
    {   //  OOPS! Log, but suppress
        qCritical() << iniFile.errorString();
    }
}

void ComponentManager::saveComponentSettings()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QFile iniFile(iniFileName());
    if (iniFile.open(QIODevice::WriteOnly))
    {
        QTextStream iniStream(&iniFile);

        QList<IComponent*> allComponents =
            ComponentManager::components().values();
        std::sort(
            allComponents.begin(),
            allComponents.end(),
            [](auto a, auto b)
            {
                return a->mnemonic() < b->mnemonic();
            });
        for (IComponent * component : std::as_const(allComponents))
        {   //  Sorted by component mnemonic to simplify lookin text editor
            iniStream << "["
                      << component->mnemonic()
                      << ":"
                      << toString(component->version())
                      << "]"
                      << Qt::endl;

            QList<AbstractSetting*> componentSettings =
                component->settings()->settings().values();
            std::sort(
                componentSettings.begin(),
                componentSettings.end(),
                [](auto a, auto b)
                {
                    return a->mnemonic() < b->mnemonic();
                });
            for (AbstractSetting * setting : std::as_const(componentSettings))
            {   //  Sorted by setting mnemonic to simplify lookin text editor
                iniStream << setting->mnemonic()
                          << "="
                          << setting->valueString()
                          << Qt::endl;
            }
            iniStream << Qt::endl;
        }
    }
    else
    {   //  OOPS! Log, but suppress
        qCritical() << iniFile.errorString();
    }
}

//////////
//  Implementation helpers
ComponentManager::_Impl * ComponentManager::_impl()
{
    static _Impl impl;

    return &impl;
}

void ComponentManager::_loadLibrary(const QString & fileName)
{
    _Impl * impl = _impl();

    //  Load file as a library.
    //  NOTE, that we don't bother checking if a DLL has already
    //  been loaded earlier - it it has, then its repeated loading
    //  will define no new Components (because they were all registered
    //  by static Component::Registrators when the DLL was loaded
    //  for the 1st time), so we can safely undo the re-load.
    qsizetype numComponents = impl->registry.size();
    QLibrary library(fileName);
    if (!library.load())
    {   // OOPS! Can't!
        return;
    }
    //  Upon successful load, the number of registered
    //  Components MAY go up - otherwise we can just
    //  unload the livrary we've just loaded
    if (impl->registry.size() == numComponents)
    {   //  The library defines no new Components
        library.unload();
        return;
    }
}

//  End of emuone-util/ComponentManager.cpp
