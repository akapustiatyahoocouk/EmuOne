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
    _refreshMruList();

    //  Reopen known VMs and restore "current" VM
    for (auto kvm : Component::Settings::instance()->recentVirtualMachines.value())
    {
        try
        {
            _openVirtualMachine(kvm.location());    //  May throw
        }
        catch (const emuone::util::Exception & ex)
        {   //  OOPS! Log, but ignore
            qCritical() << ex;
        }
    }
    for (auto vm : virtualMachines())
    {
        if (vm->location() == Component::Settings::instance()->currentVirtualMachineLocation)
        {   //  This one!
            setCurrentVirtualMachine(vm);
            break;
        }
    }

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
    _ui->actionStart->setEnabled(
        vm != nullptr && vm->isStopped());
    _ui->actionStop->setEnabled(
        vm != nullptr && !vm->isStopped());
    _ui->actionSuspend->setEnabled(
        vm != nullptr && vm->isRunning() && vm->isPersistable());
    _ui->actionResume->setEnabled(
        vm != nullptr && vm->isSuspended());
    _ui->actionReset->setEnabled(
        vm != nullptr && vm->isRunning());
    _ui->actionConfigure->setEnabled(
        vm != nullptr && vm->isStopped());
    _ui->actionFullScreen->setEnabled(
        vm != nullptr && vm->isRunning());

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

void MainFrame::_refreshMruList()
{
    QMenu * menu = _ui->actionRecentVirtualMachines->menu();
    if (menu == nullptr)
    {
        menu = new QMenu();
        _ui->actionRecentVirtualMachines->setMenu(menu);
    }
    menu->clear();

    KnownVirtualMachines mru =
        Component::Settings::instance()->recentVirtualMachines;
    for (int i = 0; i < mru.size() && i < 9; i++)
    {
        QAction * action = menu->addAction(
            mru[i].architecture()->smallIcon(),
            "&" + QString(QChar('1' + i)) + " - " +
                mru[i].name() + " [" + mru[i].location() + "]");
        auto location = mru[i].location();
        connect(
            action,
            &QAction::triggered,
            this,
            [&, location]()
            {
                try
                {
                    auto vm = _openVirtualMachine(location);  //  may throw
                    setCurrentVirtualMachine(vm);
                }
                catch (const emuone::util::Exception & ex)
                {
                    qCritical() << ex;
                    //  TODO Ask user to remove entry from MRU list
                }
            });
    }
    _ui->actionRecentVirtualMachines->setEnabled(!menu->isEmpty());
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
    _ui->tabWidget->setTabIcon(index,vm->smallIcon());
    refresh();
    //  Listen to VM state changes
    connect(vm,
            &emuone::core::VirtualMachine::stateChanged,
            this,
            &MainFrame::_virtualMachineStateChanged,
            Qt::ConnectionType::QueuedConnection);
    //  Done
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

void MainFrame::_tabWidgetCurrentChanged(int)
{
    auto vm = currentVirtualMachine();
    Component::Settings::instance()->currentVirtualMachineLocation =
        (vm != nullptr) ? vm->location() : "";
    refresh();
}

void MainFrame::_virtualMachineStateChanged(emuone::core::VirtualMachine * /*virtualMachine*/)
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
            auto vm = _openVirtualMachine(dlg.virtualMachineLocation());    //  may throw
            setCurrentVirtualMachine(vm);
            //  Update MRU list and we're done
            Component::Settings::instance()->addRecentVirtualMachine(
                KnownVirtualMachine(
                    vm->architecture(),
                    vm->type(),
                    vm->name(),
                    vm->location()));
            _refreshMruList();
            refresh();
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
        auto vm = _openVirtualMachine(path);    //  may throw
        setCurrentVirtualMachine(vm);
        //  Update MRU list and we're done
        Component::Settings::instance()->addRecentVirtualMachine(
            KnownVirtualMachine(
                vm->architecture(),
                vm->type(),
                vm->name(),
                vm->location()));
        _refreshMruList();
        refresh();
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

void MainFrame::_onActionStart()
{
    if (auto vm = currentVirtualMachine())
    {
        try
        {
            QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            vm->start();
            QGuiApplication::restoreOverrideCursor();
        }
        catch (const emuone::util::Exception & ex)
        {
            QGuiApplication::restoreOverrideCursor();
            qCritical() << ex;
            QMessageBox::critical(this, "ERROR", ex.errorMessage());
        }
        catch (...)
        {   //  Must still clean up
            QGuiApplication::restoreOverrideCursor();
        }
    }
}

void MainFrame::_onActionStop()
{
    if (auto vm = currentVirtualMachine())
    {
        try
        {
            QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            vm->stop();
            QGuiApplication::restoreOverrideCursor();
        }
        catch (const emuone::util::Exception & ex)
        {
            QGuiApplication::restoreOverrideCursor();
            qCritical() << ex;
            QMessageBox::critical(this, "ERROR", ex.errorMessage());
        }
        catch (...)
        {   //  Must still clean up
            QGuiApplication::restoreOverrideCursor();
        }
    }
}

void MainFrame::_onActionReset()
{
    QMessageBox::critical(this, "ERROR", "Not yet implemented");
}

void MainFrame::_onActionConfigure()
{
    if (auto vm = currentVirtualMachine();
        vm != nullptr && vm->isStopped())
    {
        ConfigureVirtualMachineDialog dlg(this, vm);
        if (dlg.doModal() == ConfigureVirtualMachineDialog::Result::Ok)
        {   //  TODO ?
        }
    }   //  else do nothing
}

void MainFrame::_onActionFullScreen()
{
    QMessageBox::critical(this, "ERROR", "Not yet implemented");
}

void MainFrame::_onActionAbout()
{
    QMessageBox::critical(this, "ERROR", "Not yet implemented");
}

//  End of emuone/MainFrame.cpp
