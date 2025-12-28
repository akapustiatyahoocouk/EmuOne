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

auto MainFrame::virtualMachines(
    ) const -> emuone::core::VirtualMachines
{
    emuone::core::VirtualMachines result;
    for (int i = 1; i < _ui->tabWidget->count(); i++)
    {
         auto page =
            dynamic_cast<VirtualMachinePage*>(
                _ui->tabWidget->widget(i));
        Q_ASSERT(page != nullptr);
         result.append(page->virtualMachine());
    }
    return result;
}

auto MainFrame::currentVirtualMachine(
    ) const -> emuone::core::VirtualMachine *
{
    if (auto page =
        dynamic_cast<VirtualMachinePage*>(
            _ui->tabWidget->currentWidget()))
    {
        return page->virtualMachine();
    }
    return nullptr;
}

void MainFrame::setCurrentVirtualMachine(
        emuone::core::VirtualMachine * virtualMachine
    )
{
    if (virtualMachine == nullptr)
    {
        _ui->tabWidget->setCurrentIndex(0);
    }
    else
    {
        for (int i = 1; i < _ui->tabWidget->count(); i++)
        {
            auto page =
                dynamic_cast<VirtualMachinePage*>(
                    _ui->tabWidget->widget(i));
            Q_ASSERT(page != nullptr);
            if (page->virtualMachine() == virtualMachine)
            {   //  This one!
                _ui->tabWidget->setCurrentIndex(i);
                break;
            }
        }
    }
    refresh();
}

void MainFrame::refresh()
{
    auto vm = currentVirtualMachine();

    //  TODO title, status bar

    //  Actions
    _ui->actionCloseVirtualMachine->setEnabled(vm != nullptr);

    //  Pages
    for (int i = 1; i < _ui->tabWidget->count(); i++)
    {
        auto page =
            dynamic_cast<VirtualMachinePage*>(
                _ui->tabWidget->widget(i));
        Q_ASSERT(page != nullptr);
        page->refresh();
    }
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

auto MainFrame::_openVirtualMachine(
        const QString & location
    ) -> emuone::core::VirtualMachine *
{
    //  Is a VM with the same location already open ?
    for (auto vm : virtualMachines())
    {
        if (vm->location() == location)
        {   //  Yes - nothing to be done
            return vm;
        }
    }
    //  Open and set up the UI
    auto vm = emuone::core::VirtualMachine::load(location); //  may throw
    auto page = new VirtualMachinePage(this, vm);
    int index = _ui->tabWidget->addTab(page, vm->name());
    _ui->tabWidget->setTabIcon(
        index,
        QPixmap::fromImage(vm->architecture()->smallImage()));
    refresh();
    return vm;
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
{
    refresh();
}

void MainFrame::_onActionNewVirtualMachine()
{
    NewVirtualMachineDialog dlg(this);
    if (dlg.doModal() == NewVirtualMachineDialog::Result::Ok)
    {   //  VM created - open it now
        try
        {
            setCurrentVirtualMachine(
                _openVirtualMachine(dlg.virtualMachineLocation())); //  may throw
        }
        catch (const emuone::util::Exception & ex)
        {
            qCritical() << ex;
            //  TODO MessageDialog
            QMessageBox::critical(this, "ERROR", ex.errorMessage());
        }
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
        setCurrentVirtualMachine(
            _openVirtualMachine(path)); //  may throw
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
