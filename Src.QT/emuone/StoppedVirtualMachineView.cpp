//
//  emuone/StoppedVirtualMachineView.cpp - emuone::StoppedVirtualMachineView class implementation
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
#include "ui_StoppedVirtualMachineView.h"
using namespace emuone;

//////////
//  Constrution/estruction
StoppedVirtualMachineView::StoppedVirtualMachineView(
        QWidget * parent,
        emuone::core::VirtualMachine * virtualMachine
    ) : QWidget(parent),
        _virtualMachine(virtualMachine),
        _ui(new Ui::StoppedVirtualMachineView)
{
    Q_ASSERT(_virtualMachine != nullptr);

    _ui->setupUi(this);

    _ui->configurationTreeWidget->setItemDelegate(
        new AutoToolTipDelegate(_ui->configurationTreeWidget));

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
    for (auto cc : std::as_const(componentCategories))
    {
        auto item = new QTreeWidgetItem();
        item->setText(0, cc->displayName());
        item->setIcon(0, cc->smallIcon());
        item->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(cc));
        _ui->configurationTreeWidget->addTopLevelItem(item);
    }

    //  Done
    refresh();
    _ui->configurationTreeWidget->expandAll();
}

StoppedVirtualMachineView::~StoppedVirtualMachineView()
{
    delete _ui;
}

//////////
//  Operations
void StoppedVirtualMachineView::refresh()
{
    _ui->nameValueLabel->setText(_virtualMachine->name());
    _ui->locationValueLabel->setText(_virtualMachine->location());
    _ui->architectureValueLabel->setText(_virtualMachine->architecture()->displayName());
    _ui->typeValueLabel->setText(_virtualMachine->type()->displayName());

    _refreshComponentsTree();
}

//////////
//  Implementation helpers
void StoppedVirtualMachineView::_refreshComponentsTree()
{
    //  Top-level (category) items do not change
    for (int i = 0; i < _ui->configurationTreeWidget->topLevelItemCount(); i++)
    {
        _refreshComponentCategoryItem(_ui->configurationTreeWidget->topLevelItem(i));
    }
}

void StoppedVirtualMachineView::_refreshComponentCategoryItem(QTreeWidgetItem * categoryItem)
{
    auto category = categoryItem->data(0, Qt::ItemDataRole::UserRole).value<emuone::core::IComponentCategory*>();
    auto components = _virtualMachine->components().values();
    components.removeIf([category](auto c) { return c->type()->category() != category; });
    std::sort(
        components.begin(),
        components.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    //  Make sure there is a proper number of Component items
    while (categoryItem->childCount() < components.size())
    {   //  Too few child items
        auto childItem = new QTreeWidgetItem();
        categoryItem->addChild(childItem);
    }
    while (categoryItem->childCount() > components.size())
    {   //  Too many child items
        delete categoryItem->takeChild(categoryItem->childCount() - 1);
    }
    //  Make sure ech component item is property set up
    for (int i = 0; i < categoryItem->childCount(); i++)
    {
        categoryItem->child(i)->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(components[i]));
        _refreshComponentItem(categoryItem->child(i));
    }
}

void StoppedVirtualMachineView::_refreshComponentItem(QTreeWidgetItem * componentItem)
{
    auto component = componentItem->data(0, Qt::ItemDataRole::UserRole).value<emuone::core::IComponent*>();
    componentItem->setText(0, component->displayName());
    componentItem->setIcon(0, component->smallIcon());
}

//  End of emuone/StoppedVirtualMachineView.cpp
