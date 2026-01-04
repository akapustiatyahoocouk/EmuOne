//
//  emuone-hades/KernelConsoleDisplaySurface.cpp - emuone::hades::kernel::KernelConsoleDisplaySurface class implementation
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
#include "ui_KernelConsoleDisplaySurface.h"
using namespace emuone::hades::kernel;

//////////
//  Construction/destruction
KernelConsoleDisplaySurface::KernelConsoleDisplaySurface(
        QWidget * parent,
        Kernel * kernel
    ) : emuone::core::DisplaySurface(parent, kernel),
        _kernel(kernel),
        _ui(new Ui::KernelConsoleDisplaySurface)
{
    _ui->setupUi(this);
}

KernelConsoleDisplaySurface::~KernelConsoleDisplaySurface()
{
    delete _ui;
}

//  End of emuone-hades/KernelConsoleDisplaySurface.cpp
