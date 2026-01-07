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
        _ui(new Ui::KernelConsoleDisplaySurface),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _kernel->_kernelConsoleDisplaySurfaces.insert(this);

    //  Use fixed-size font for console
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    _ui->plainTextEdit->setFont(fixedFont);

    //  Handle refreshes
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &KernelConsoleDisplaySurface::_refreshTimerTimeout);
    _refreshTimer.start(250);
}

KernelConsoleDisplaySurface::~KernelConsoleDisplaySurface()
{
    delete _ui;
    _kernel->_kernelConsoleDisplaySurfaces.remove(this);
}

//////////
//  Signal handlers
void KernelConsoleDisplaySurface::_refreshTimerTimeout()
{   //  TODO use some sort of "update sequence number" to
    //  avoid the full text compare on each timer tick
    QString s;
    {
        emuone::util::Lock _(_kernel->kernelGuard);
        s = _kernel->_kernelConsoleContent.join('\n') + '\n';
    }
    if (_ui->plainTextEdit->toPlainText() != s)
    {
        _ui->plainTextEdit->setPlainText(s);
        _ui->plainTextEdit->moveCursor(QTextCursor::End);
        _ui->plainTextEdit->ensureCursorVisible(); // Optional: ensures visibility, though End operation usually handles this
    }
}

//  End of emuone-hades/KernelConsoleDisplaySurface.cpp
