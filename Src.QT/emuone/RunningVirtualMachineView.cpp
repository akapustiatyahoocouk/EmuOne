//
//  emuone/RunningVirtualMachineView.cpp - emuone::RunningVirtualMachineView class implementation
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
#include "ui_RunningVirtualMachineView.h"
using namespace emuone;

//////////
//  Constrution/estruction
RunningVirtualMachineView::RunningVirtualMachineView(
        QWidget * parent,
        emuone::core::VirtualMachine * virtualMachine
    ) : QWidget(parent),
        _virtualMachine(virtualMachine),
        _ui(new Ui::RunningVirtualMachineView)
{
    Q_ASSERT(_virtualMachine != nullptr);

    _ui->setupUi(this);
}

RunningVirtualMachineView::~RunningVirtualMachineView()
{
    if (_uiCreated)
    {
        _destroyUi();
    }
    delete _ui;
}

//////////
//  Operations
void RunningVirtualMachineView::refresh()
{
    //  if the VM is not Running, make sure there are np leftover UIs
    if (!_virtualMachine->isRunning())
    {
        if (_uiCreated)
        {
            _destroyUi();
            _uiCreated = false;
        }
        return;
    }
    //  The VM is Running - create UIs...
    if (!_uiCreated)
    {   //  ONCE!
        _createUi();
        _uiCreated = true;
    }
}

//////////
//  Implementation helpers
void RunningVirtualMachineView::_createUi()
{
    Q_ASSERT(_componentUis.isEmpty());
    Q_ASSERT(_adaptorUis.isEmpty());
    Q_ASSERT(_displaySurfaces.isEmpty());

    //  Create UIs
    for (auto c : _virtualMachine->components())
    {
        if (auto ui = c->createUi())
        {
            _componentUis[c] = ui;
        }
    }
    for (auto c : _virtualMachine->adaptedComponents())
    {
        auto a = _virtualMachine->findAdaptor(c);
        Q_ASSERT(a != nullptr);
        if (auto ui = a->createUi())
        {
            _adaptorUis[a] = ui;
        }
    }

    //  Register UI artefacts
    for (auto ui : _componentUis.values())
    {
        for (auto ds : ui->displaySurfacess())
        {
            _displaySurfaces.append(ds);
        }
    }
    for (auto ui : _adaptorUis.values())
    {
        for (auto ds : ui->displaySurfacess())
        {
            _displaySurfaces.append(ds);
        }
    }
    std::sort(
        _displaySurfaces.begin(),
        _displaySurfaces.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });

    //  Create tabs for DisplaySurfaces
    for (auto ds : _displaySurfaces)
    {
        _ui->tabWidget->addTab(
            ds,
            ds->component()->type()->smallIcon(),
            ds->displayName());
    }
}

void RunningVirtualMachineView::_destroyUi()
{
    //  Clear DisplaySurface tabs
    _ui->tabWidget->clear();

    //  Destroy all UIs.
    //  GUI artuifacts are destroyed by the UIs
    for (auto ui : _adaptorUis.values())
    {
        delete ui;  //  removes artefacts from GUI hierarchy
    }
    _adaptorUis.clear();
    for (auto ui : _componentUis.values())
    {
        delete ui;  //  removes artefacts from GUI hierarchy
    }
    _componentUis.clear();

    _displaySurfaces.clear();
}

//  End of emuone/RunningVirtualMachineView.cpp
