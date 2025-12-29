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

    //  Populate the "architecture" combo box
    auto architectures = emuone::core::ArchitectureManager::all().values();
    std::sort(
        architectures.begin(),
        architectures.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto a : std::as_const(architectures))
    {
        _ui->architectureComboBox->addItem(
            a->smallIcon(),
            a->displayName(),
            QVariant::fromValue(a));
    }

    //  Populate the "virtualMachineType" combo box
    auto virtualMachineTypes = emuone::core::VirtualMachineTypeManager::all().values();
    std::sort(
        virtualMachineTypes.begin(),
        virtualMachineTypes.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto t : std::as_const(virtualMachineTypes))
    {
        _ui->virtualMachineTypeComboBox->addItem(
            t->smallIcon(),
            t->displayName(),
            QVariant::fromValue(t));
    }

    //  Populate the "templates" combo box
    _refillTemplatesComboBox();

    //  Set static control values
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values

    //  Done
    _refresh();
    adjustSize();
    _ui->nameLabel->setFocus();
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

QString NewVirtualMachineDialog::virtualMachineLocation() const
{
    return _virtualMachineLocation;
}

//////////
//  Implementation
auto NewVirtualMachineDialog::_selectedArchitecture() -> emuone::core::IArchitecture *
{
    return (_ui->architectureComboBox->currentIndex() != -1) ?
           _ui->architectureComboBox->currentData().value<emuone::core::IArchitecture*>() :
                nullptr;
}

void NewVirtualMachineDialog::_setSelectedArchitecture(emuone::core::IArchitecture * architecture)
{
    for (int i = 0; i < _ui->architectureComboBox->count(); i++)
    {
        if (_ui->architectureComboBox->itemData(i).value<emuone::core::IArchitecture*>() == architecture)
        {
            _ui->architectureComboBox->setCurrentIndex(i);
            break;
        }
    }
}

auto NewVirtualMachineDialog::_selectedType() -> emuone::core::IVirtualMachineType *
{
    return (_ui->virtualMachineTypeComboBox->currentIndex() != -1) ?
               _ui->virtualMachineTypeComboBox->currentData().value<emuone::core::IVirtualMachineType*>() :
               nullptr;
}

void NewVirtualMachineDialog::_setSelectedType(emuone::core::IVirtualMachineType * virtualMachineType)
{
    for (int i = 0; i < _ui->virtualMachineTypeComboBox->count(); i++)
    {
        if (_ui->virtualMachineTypeComboBox->itemData(i).value<emuone::core::IVirtualMachineType*>() == virtualMachineType)
        {
            _ui->virtualMachineTypeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

auto NewVirtualMachineDialog::_selectedTemplate(
    ) -> emuone::core::IVirtualMachineTemplate *
{
    return (_ui->templateComboBox->currentIndex() != -1) ?
               _ui->templateComboBox->currentData().value<emuone::core::IVirtualMachineTemplate*>() :
               nullptr;
}

void NewVirtualMachineDialog::_setSelectedTemplate(
        emuone::core::IVirtualMachineTemplate * virtualMachineTemplate
    )
{
    for (int i = 0; i < _ui->templateComboBox->count(); i++)
    {
        if (_ui->templateComboBox->itemData(i).value<emuone::core::IVirtualMachineTemplate*>() == virtualMachineTemplate)
        {
            _ui->templateComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void NewVirtualMachineDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        emuone::core::VirtualMachine::isValidName(_ui->nameLineEdit->text()) &&
        //_selectedArchitecture() != nullptr &&
        //_selectedVirtualMachineType() != nullptr &&
        !_ui->locationLineEdit->text().isEmpty());
}

void NewVirtualMachineDialog::_refillTemplatesComboBox()
{
    static const emuone::core::IVirtualMachineTemplate * NoTemplate = nullptr;

    _ui->templateComboBox->clear();
    _ui->templateComboBox->addItem(
        "-",
        QVariant::fromValue(NoTemplate));

    auto templates = emuone::core::VirtualMachineTemplateManager::all().values();
    std::sort(
        templates.begin(),
        templates.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto t : std::as_const(templates))
    {
        if (t->architecture() == _selectedArchitecture() &&
            t->virtualMachineType() == _selectedType())
        {
            _ui->templateComboBox->addItem(
                t->smallIcon(),
                t->displayName(),
                QVariant::fromValue(t));
        }
    }
}

//////////
//  Signal handlers
void NewVirtualMachineDialog::_nameLineEditTextChanged(QString)
{
    _refresh();
}

void NewVirtualMachineDialog::_architectureComboBoxCurrentIndexChanged(int)
{
    _refillTemplatesComboBox();
    _refresh();
}

void NewVirtualMachineDialog::_virtualMachineTypeComboBoxCurrentIndexChanged(int)
{
    _refillTemplatesComboBox();
    _refresh();
}

void NewVirtualMachineDialog::_templateComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void NewVirtualMachineDialog::_locationLineEditTextChanged(QString)
{
    _refresh();
}

void NewVirtualMachineDialog::_browsePushButtonClicked()
{
    QString path =
        QFileDialog::getSaveFileName(
            this,
            "New virtual machine",
            /*dir =*/ QString(),
            "EmuOne files (*" +
            emuone::core::VirtualMachine::PreferredExtension +
            ");;All files (*.*)");
    if (!path.isEmpty())
    {
        if (QFileInfo(path).suffix().isEmpty())
        {   //  On e.g. Linux we may need to auto-add the extension
            path += emuone::core::VirtualMachine::PreferredExtension;
        }
        _ui->locationLineEdit->setText(path);
        _refresh();
    }
}

void NewVirtualMachineDialog::accept()
{
    if (_selectedTemplate() == nullptr)
    {   //  No template - create empty VM
        auto vm =   //  auto-deleted when block exits
            std::make_unique<emuone::core::VirtualMachine>(
                _ui->nameLineEdit->text().trimmed(),
                _ui->locationLineEdit->text(),
            _selectedArchitecture(),
            _selectedType());
        vm->save(); //  TODO may throw
        _virtualMachineLocation = vm->location();
        done(int(Result::Ok));
    }
    else
    {   //  TODO implement
        Q_ASSERT(false);
    }
}

void NewVirtualMachineDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of emuone/NewVirtualMachineDialog.cpp
