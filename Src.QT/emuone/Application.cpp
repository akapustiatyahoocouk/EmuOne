//
//  emuone/Application.cpp - emuone::Application class implementation
//
//  EmuOne
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
#include "emuone/API.hpp"
using namespace emuone;

//////////
//  Construction/destruction
Application::Application(int & argc, char ** argv)
    :   QApplication(argc, argv)
{
    /*  TODO uncomment & use
    setApplicationName(emuone::util::ProductInformation::applicationName());
    setApplicationVersion(emuone::util::ProductInformation::applicationVersion().toString());
    //  Don't setApplicationDisplayName() - will mess up dialog titles!
    setOrganizationName(emuone::util::ProductInformation::organizationName());
    setOrganizationDomain(emuone::util::ProductInformation::organizationDomain());
    */
}

Application::~Application()
{
}

//////////
//  QApplication
int Application::exec()
{
    try
    {
        _initialize();
        int exitCode = QApplication::exec();    //  may throw
        _cleanup();
        return exitCode;
    }
    catch (...)
    {   //  OOPS! Cleanup & re-throw
        _cleanup();
        throw;
    }
}

//////////
//  QGuiApplication
/*  TODO uncomment
bool Application::notify(QObject * receiver, QEvent * event)
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const emuone::util::Exception & ex)
    {
        qCritical() << ex;
        emuone::gui::ErrorDialog::show(QApplication::activeWindow(), ex);
        return false;
    }
    catch (const emuone::util::Error & ex)
    {
        qCritical() << ex;
        emuone::gui::ErrorDialog::show(QApplication::activeWindow(), ex);
        return false;
    }
    catch (...)
    {
        qCritical() << resources->string(RSID(Errors), RID(UncaughtException));
        return false;
    }
}
*/

//////////
//  Implementation helpers
void Application::_initialize()
{
    QPixmap pm;
    pm.load(":/emuone/Resources/Images/Misc/EmuOneLarge.png");
    QIcon ic(pm);
    QGuiApplication::setWindowIcon(ic);

    emuone::util::ComponentManager::discoverComponents();
    emuone::util::ComponentManager::initializeComponents();
    emuone::util::ComponentManager::loadComponentSettings();

    //  Create & show the main UI frame
    _mainFrame = new MainFrame();
    _mainFrame->show();
}

void Application::_cleanup()
{
    //  Hide & destroy the main UI frame
    _mainFrame->hide();
    delete _mainFrame;
    _mainFrame = nullptr;

    //  Deinitialize all Components
    emuone::util::ComponentManager::saveComponentSettings();
    emuone::util::ComponentManager::deinitializeComponents();
}

//  End of emuone/Application.cpp
