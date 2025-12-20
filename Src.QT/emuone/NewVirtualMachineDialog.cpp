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

    //  Populate the "architecture" combp box
    auto architectures = emuone::core::ArchitectureManager::allArchitectures();
    QList<emuone::core::IArchitecture*> architecturesList(
        architectures.cbegin(),
        architectures.cend());
    std::sort(
        architecturesList.begin(),
        architecturesList.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto architecture : architecturesList)
    {
        _ui->architectureComboBox->addItem(
            QIcon(QPixmap::fromImage(architecture->smallImage())),
            architecture->displayName(),
            QVariant::fromValue(architecture));
    }

    //  Set static control values
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/CancelSmall.png"));
}

NewVirtualMachineDialog::~NewVirtualMachineDialog()
{
    delete _ui;
}

//////////
//  Operations
auto NewVirtualMachineDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

//////////
//  Signal handlers
void NewVirtualMachineDialog::accept()
{
    done(int(Result::Ok));
}

void NewVirtualMachineDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of emuone/NewVirtualMachineDialog.cpp
