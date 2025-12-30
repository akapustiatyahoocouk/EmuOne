//
//  emuone-hades/KernelEditor.cpp - emuone::hades::KernelEditor class implementation
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
#include "emuone-hades/API.hpp"
#include "ui_KernelEditor.h"
using namespace emuone::hades;

//////////
//  Construction/destruction
KernelEditor::KernelEditor(
        QWidget * parent,
        Kernel * kernel
    ) : ComponentEditor(parent, kernel),
        _kernel(kernel),
        _ui(new Ui::KernelEditor)
{
    _ui->setupUi(this);
}

KernelEditor::~KernelEditor()
{
    delete _ui;
}

//  End of emuone-hades/KernelEditor.cpp
