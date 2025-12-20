//
//  emuone/NewVirtualMachineDialog.cpp - emuone::NewVirtualMachineDialog class implementation
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
#include "ui_NewVirtualMachineDialog.h"
using namespace emuone;

//////////
//  Constrution/estruction
NewVirtualMachineDialog::NewVirtualMachineDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::NewVirtualMachineDialog)
{
    _ui->setupUi(this);
}

NewVirtualMachineDialog::~NewVirtualMachineDialog()
{
    delete _ui;
}

//  End of emuone/NewVirtualMachineDialog.cpp
