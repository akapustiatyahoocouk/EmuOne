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

    //  Populate the "virtualMachineType" combo box
    auto virtualMachineTypes = emuone::core::VirtualMachineTypeManager::allVirtualMachineTypes();
    QList<emuone::core::IVirtualMachineType*> virtualMachineTypesList(
        virtualMachineTypes.cbegin(),
        virtualMachineTypes.cend());
    std::sort(
        virtualMachineTypesList.begin(),
        virtualMachineTypesList.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto virtualMachineType : virtualMachineTypesList)
    {
        _ui->virtualMachineTypeComboBox->addItem(
            QIcon(QPixmap::fromImage(virtualMachineType->smallImage())),
            virtualMachineType->displayName(),
            QVariant::fromValue(virtualMachineType));
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

auto NewVirtualMachineDialog::_selectedVirtualMachineType() -> emuone::core::IVirtualMachineType *
{
    return (_ui->virtualMachineTypeComboBox->currentIndex() != -1) ?
               _ui->virtualMachineTypeComboBox->currentData().value<emuone::core::IVirtualMachineType*>() :
               nullptr;
}

void NewVirtualMachineDialog::_setSelectedVirtualMachineType(emuone::core::IVirtualMachineType * virtualMachineType)
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

auto NewVirtualMachineDialog::_selectedTemplate() -> emuone::core::ITemplate *
{
    return (_ui->templateComboBox->currentIndex() != -1) ?
               _ui->templateComboBox->currentData().value<emuone::core::ITemplate*>() :
               nullptr;
}

void NewVirtualMachineDialog::_setSelectedTemplate(emuone::core::ITemplate * template_)
{
    for (int i = 0; i < _ui->templateComboBox->count(); i++)
    {
        if (_ui->templateComboBox->itemData(i).value<emuone::core::ITemplate*>() == template_)
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
    static const emuone::core::ITemplate * NoTemplate = nullptr;

    _ui->templateComboBox->clear();
    _ui->templateComboBox->addItem(
        "-",
        QVariant::fromValue(NoTemplate));

    auto templates = emuone::core::TemplateManager::allTemplates();
    QList<emuone::core::ITemplate*> templatesList(
        templates.cbegin(),
        templates.cend());
    std::sort(
        templatesList.begin(),
        templatesList.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto template_ : templatesList)
    {
        if (template_->architecture() == _selectedArchitecture() &&
            template_->virtualMachineType() == _selectedVirtualMachineType())
        {
            _ui->templateComboBox->addItem(
                QIcon(QPixmap::fromImage(template_->smallImage())),
                template_->displayName(),
                QVariant::fromValue(template_));
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
    done(int(Result::Ok));
}

void NewVirtualMachineDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of emuone/NewVirtualMachineDialog.cpp
