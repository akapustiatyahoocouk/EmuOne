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
    delete _ui;
}

//////////
//  Operations
void RunningVirtualMachineView::refresh()
{
}

//  End of emuone/RunningVirtualMachineView.cpp
