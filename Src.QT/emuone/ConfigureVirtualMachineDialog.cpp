//
//  emuone/ConfigureVirtualMachineDialog.cpp - emuone::ConfigureVirtualMachineDialog class implementation
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
#include "ui_ConfigureVirtualMachineDialog.h"
using namespace emuone;

//////////
//  Constrution/estruction
ConfigureVirtualMachineDialog::ConfigureVirtualMachineDialog(
        QWidget * parent,
        emuone::core::VirtualMachine * virtualMachine
    ) : QDialog(parent),
        _virtualMachine(virtualMachine),
        _ui(new Ui::ConfigureVirtualMachineDialog)
{
    Q_ASSERT(_virtualMachine != nullptr &&
             _virtualMachine->isStopped());
    _ui->setupUi(this);

    //  Must save original configurations of VM components in
    //  case user makes some changes and then cancels the dialog
    for (auto c : _virtualMachine->components())
    {
        _savedComponentConfigurations[c] = _saveConfiguration(c);
    }
    for (auto c : _virtualMachine->adaptedComponents())
    {
        _savedComponentAdaptorConfigurations[c] =
            _saveConfiguration(_virtualMachine->findAdaptor(c));
        _preferredAdaptorTypes[c] =
            _virtualMachine->findAdaptor(c)->type();
    }

    //  Populte components tree widget with category nodes
    auto componentCategories =
        emuone::core::ComponentCategoryManager::all().values();
    std::sort(
        componentCategories.begin(),
        componentCategories.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (auto cc : componentCategories)
    {
        auto item = new QTreeWidgetItem();
        item->setText(0, cc->displayName());
        item->setIcon(0, cc->smallIcon());
        item->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(cc));
        _ui->componentsTreeWidget->addTopLevelItem(item);
    }

    //  Set static control values
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/CancelSmall.png"));

    //  Create dynamic controls
    _editorsPanelLayout = new QStackedLayout(this);
    _noPropertiesLabel = new QLabel(_ui->editorsPanel);
    _editorsPanelLayout->addWidget(_noPropertiesLabel);
    _ui->editorsPanel->setLayout(_editorsPanelLayout);

    //  Set editable control values
    _ui->nameLineEdit->setText(
        _virtualMachine->name());
    _ui->locationValueLabel->setText(
        _virtualMachine->location());
    _ui->architectureValueLabel->setText(
        _virtualMachine->architecture()->displayName());
    _ui->typeValueLabel->setText(
        _virtualMachine->type()->displayName());

    _noPropertiesLabel->setText("No properties to edit");
    _noPropertiesLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter |
                                     Qt::AlignmentFlag::AlignVCenter);
    //  Done
    _refresh();
    adjustSize();
}

ConfigureVirtualMachineDialog::~ConfigureVirtualMachineDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ConfigureVirtualMachineDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void ConfigureVirtualMachineDialog::_refresh()
{
    //  TODOS
}

QString ConfigureVirtualMachineDialog::_saveConfiguration(
        emuone::core::IComponent * component
    )
{
    Q_ASSERT(component != nullptr);

    QDomDocument document;
    auto componentElement = document.createElement("Component");
    document.appendChild(componentElement);
    component->saveConfiguration(componentElement);
    return document.toString(4);
}

QString ConfigureVirtualMachineDialog::_saveConfiguration(
        emuone::core::IComponentAdaptor * componentAdaptor
    )
{
    Q_ASSERT(componentAdaptor != nullptr);

    QDomDocument document;
    auto componentAdaptorElement = document.createElement("ComponentAdaptor");
    document.appendChild(componentAdaptorElement);
    componentAdaptor->saveConfiguration(componentAdaptorElement);
    return document.toString(4);
}

void ConfigureVirtualMachineDialog::_restoreConfiguration(
        emuone::core::IComponent * component,
        const QString & configurationXml
    )
{
    Q_ASSERT(component != nullptr);

    QDomDocument document;
    document.setContent(configurationXml);
    Q_ASSERT(!document.documentElement().isNull() &&
             document.documentElement().tagName() == "Component");
    component->restoreConfiguration(document.documentElement());
}

void ConfigureVirtualMachineDialog::_restoreConfiguration(
        emuone::core::IComponentAdaptor * componentAdaptor,
        const QString & configurationXml
    )
{
    Q_ASSERT(componentAdaptor != nullptr);

    QDomDocument document;
    document.setContent(configurationXml);
    Q_ASSERT(!document.documentElement().isNull() &&
             document.documentElement().tagName() == "ComponentAdaptor");
    componentAdaptor->restoreConfiguration(document.documentElement());
}

//////////
//  Signal handlers
void ConfigureVirtualMachineDialog::_addComponentPushButtonClicked()
{
}

void ConfigureVirtualMachineDialog::_removeComponentPushButtonClicked()
{
}

void ConfigureVirtualMachineDialog::accept()
{   //  Changes already made to VM.
    //  The components that were removed from VM are now
    //  unbound and must be delete'd
    for (auto c : _removedComponents)
    {
        Q_ASSERT(!c->isBound());
        delete c;
    }
    done(int(Result::Ok));
}

void ConfigureVirtualMachineDialog::reject()
{   //  Rollback changes made to VM.
    //  All components added to VM must be removed and deleted
    for (auto c : std::as_const(_addedComponents))
    {
        _virtualMachine->removeComponent(c);    //  deleted adaptor if there IS one
        delete c;
    }
    //  All components removed from VM must be restored
    for (auto c : std::as_const(_removedComponents))
    {
        if (auto cat = _preferredAdaptorTypes.value(c, nullptr))
        {   //  Component was adapted
            _virtualMachine->addComponent(c, cat);
        }
        else
        {   //  Component was native
            _virtualMachine->addComponent(c);
        }
    }
    //  Configuration of all VM components must be restored
    for (auto c : _virtualMachine->components())
    {
        Q_ASSERT(_savedComponentConfigurations.contains(c));
        _restoreConfiguration(c, _savedComponentConfigurations[c]);
    }
    for (auto c : _virtualMachine->adaptedComponents())
    {
        auto a = _virtualMachine->findAdaptor(c);
        Q_ASSERT(a != nullptr);
        Q_ASSERT(_savedComponentAdaptorConfigurations.contains(c));
        _restoreConfiguration(a, _savedComponentAdaptorConfigurations[c]);
    }
    //  We're done
    done(int(Result::Cancel));
}

//  End of emuone/ConfigureVirtualMachineDialog.cpp
