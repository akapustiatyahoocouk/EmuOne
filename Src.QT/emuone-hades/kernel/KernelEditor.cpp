//
//  emuone-hades/KernelEditor.cpp - emuone::hades::kernel::KernelEditor class implementation
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
using namespace emuone::hades::kernel;

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

    _loadControlValues();
    _constructed = true;
}

KernelEditor::~KernelEditor()
{
    delete _ui;
}

//////////
//  emuone::core::ComponentEditor
bool KernelEditor::isValid() const
{
    return emuone::util::fromString<QVersionNumber>(_ui->kernelVersionLneEdit->text(), QVersionNumber(0, 0, 0)) > QVersionNumber(0, 0, 0);
}

//////////
//  Implementation helpers
void KernelEditor::_loadControlValues()
{
    _ui->kernelVersionLneEdit->setText(
        emuone::util::toString(_kernel->version()));
}

void KernelEditor::_saveControlValues() const
{
    if (isValid())
    {
        _kernel->setVersion(
            emuone::util::fromString<QVersionNumber>(_ui->kernelVersionLneEdit->text()));
    }
}

//////////
//  Signal handlers
void KernelEditor::_kernelVersionLineEditTextChanged(QString)
{
    if (_constructed)
    {
        _saveControlValues();
        emit valueChanged();
    }
}

//  End of emuone-hades/KernelEditor.cpp
