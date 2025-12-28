//
//  emuone/MainFrame.cpp - emuone::MainFrame class implementation
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
#include "emuone/API.hpp"
#include "ui_MainFrame.h"
using namespace emuone;

//////////
//  Constrution/estruction
MainFrame::MainFrame()
    :   QMainWindow(),
        _ui(new Ui::MainFrame),
        _trackPositionTimer(this),
        _savePositionTimer(this),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    this->setMinimumSize(MinimumSize);

    _loadPosition();

    //  Reopen known VMs

    //  Set up signal handlers
    _trackPositionTimer.setSingleShot(true);
    _savePositionTimer.setSingleShot(true);
    connect(&_trackPositionTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_trackPositionTimerTimeout);
    connect(&_savePositionTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_savePositionTimerTimeout);

    //  Done
    refresh();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

MainFrame::~MainFrame()
{
    //  Stop & destroy all VMs
    for (auto vm : _virtualMachines)
    {
        delete vm;
    }
    //  Cleanuo & done
    delete _ui;
}

//////////
//  QWidget
void MainFrame::moveEvent(QMoveEvent * event)
{
    QMainWindow::moveEvent(event);
    _savePositionTimer.start(500);
}

void MainFrame::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    _savePositionTimer.start(500);
}

void MainFrame::closeEvent(QCloseEvent * event)
{
    event->ignore();
    _onActionExit();
}

void MainFrame::show()
{
    if (!this->isVisible())
    {
        QMainWindow::show();
        //  Under X11 it will be some time before the window manager
        //  stabilizes the frame position; it is at THAT time that
        //  frame position needs to be loaded and tracjed
        _trackPositionTimer.start(500);
    }
}

void MainFrame::hide()
{
    if (this->isVisible())
    {
        _trackPosition = false;
        QMainWindow::hide();
    }
}

void MainFrame::refresh()
{
    //  TODO title ? states ? status bar ?
}

//////////
//  Implementation
void MainFrame::_loadPosition()
{
    restoreGeometry(Component::Settings::instance()->mainFrameGeometry);
    //  Make sure the frame is not off-screen
    if (Component::Settings::instance()->mainFrameMaximized)
    {
        this->showMaximized();
    }
}

void MainFrame::_savePosition()
{
    if (_trackPosition)
    {
        if (this->isMaximized())
        {
            Component::Settings::instance()->mainFrameMaximized = true;
        }
        else if (!this->isMinimized())
        {
            Component::Settings::instance()->mainFrameGeometry = this->saveGeometry();
            Component::Settings::instance()->mainFrameMaximized = false;
        }
    }
}

void MainFrame::_openVirtualMachine(const QString & location)
{
    //  Is a VM with the same location already open ?
    for (auto vm : _virtualMachines)
    {
        if (vm->location() == location)
        {   //  Yes - nothing to be done
            return;
        }
    }
    //  Open, add...
    _virtualMachines.append(
        emuone::core::VirtualMachine::load(location));  //  may throw
    //  ...and set up the UI
}

//////////
//  Signal handlers
void MainFrame::_trackPositionTimerTimeout()
{
    _loadPosition();
    _trackPosition = true;
}

void MainFrame::_savePositionTimerTimeout()
{
    _savePosition();
}

void MainFrame::_refreshTimerTimeout()
{   //  TODO VM states ?
}

void MainFrame::_onActionNewVirtualMachine()
{
    NewVirtualMachineDialog dlg(this);
    if (dlg.doModal() == NewVirtualMachineDialog::Result::Ok)
    {   //  VM created
        //  TODO ?
    }

}

void MainFrame::_onActionOpenVirtualMachine()
{
    QString path =
        QFileDialog::getOpenFileName(
            this,
            "Open virtual machine",
            /*dir =*/ QString(),
            "EmuOne files (*" +
            emuone::core::VirtualMachine::PreferredExtension +
            ");;All files (*.*)");
    if (path.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    try
    {
        _openVirtualMachine(path);  //  may throw
    }
    catch (const emuone::util::Exception & ex)
    {
        qCritical() << ex;
        //  TODO MessageDialog
        QMessageBox::critical(this, "ERROR", ex.errorMessage());
    }
}

void MainFrame::_onActionExit()
{
    QApplication::exit(0);
}

//  End of emuone/MainFrame.cpp
