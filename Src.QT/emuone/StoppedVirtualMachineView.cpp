//
//  emuone/StoppedVirtualMachineView.cpp - emuone::StoppedVirtualMachineView class implementation
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
#include "ui_StoppedVirtualMachineView.h"
using namespace emuone;

//////////
//  Constrution/estruction
StoppedVirtualMachineView::StoppedVirtualMachineView(
        QWidget * parent,
        emuone::core::VirtualMachine * virtualMachine
    ) : QWidget(parent),
        _virtualMachine(virtualMachine),
        _ui(new Ui::StoppedVirtualMachineView)
{
    Q_ASSERT(_virtualMachine != nullptr);

    _ui->setupUi(this);

    //  Done
    refresh();
}

StoppedVirtualMachineView::~StoppedVirtualMachineView()
{
    delete _ui;
}

//////////
//  Operations
void StoppedVirtualMachineView::refresh()
{
    _ui->nameValueLabel->setText(_virtualMachine->name());
    _ui->locationValueLabel->setText(_virtualMachine->location());
    _ui->architectureValueLabel->setText(_virtualMachine->architecture()->displayName());
    _ui->typeValueLabel->setText(_virtualMachine->type()->displayName());
}

//  End of emuone/StoppedVirtualMachineView.cpp
