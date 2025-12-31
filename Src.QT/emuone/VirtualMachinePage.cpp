//
//  emuone/VirtualMachinePage.cpp - emuone::VirtualMachinePage class implementation
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
#include "ui_VirtualMachinePage.h"
using namespace emuone;

//////////
//  Constrution/estruction
VirtualMachinePage::VirtualMachinePage(
        QWidget * parent,
        emuone::core::VirtualMachine * virtualMachine
    )
    :   QWidget(parent),
        _virtualMachine(virtualMachine),    //  takes ownership
        _ui(new Ui::VirtualMachinePage)
{
    Q_ASSERT(_virtualMachine != nullptr);

    _ui->setupUi(this);

    //  Create dynamic controls
    _layout = new QStackedLayout(this);
    _stoppedView =
        new StoppedVirtualMachineView(this, _virtualMachine);
    //  TODO views
    _layout->addWidget(_stoppedView);
    this->setLayout(_layout);

    //  Done
    refresh();
}

VirtualMachinePage::~VirtualMachinePage()
{
    //  Suspend/stop the owned VM and delete it
    //  TODO suspend/stop
    delete _virtualMachine;
    //  Cleanup & we're done
    delete _ui;
}

void VirtualMachinePage::refresh()
{   //  TODO refresh all views & select ctive view
    _stoppedView->refresh();
    switch (_virtualMachine->state())
    {
        case emuone::core::VirtualMachine::State::Stopped:
            _layout->setCurrentWidget(_stoppedView);
            break;
        case emuone::core::VirtualMachine::State::Running:
            //  TODO
            break;
        case emuone::core::VirtualMachine::State::Suspended:
            Q_ASSERT(false);    //  TODO implement
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}

//  End of emuone/VirtualMachinePage.cpp
