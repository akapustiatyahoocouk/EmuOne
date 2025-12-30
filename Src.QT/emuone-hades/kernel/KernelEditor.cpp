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
}

KernelEditor::~KernelEditor()
{
    delete _ui;
}

//////////
//  emuone::core::ComponentEditor
void KernelEditor::loadControlValues()
{
    _ui->kernelVersionLneEdit->setText(
        emuone::util::toString(_kernel->version()));
}

void KernelEditor::saveControlValues() const
{
    if (isValid())
    {
        _kernel->setVersion(
            emuone::util::fromString<QVersionNumber>(_ui->kernelVersionLneEdit->text()));
    }
}

bool KernelEditor::isValid() const
{
    return emuone::util::fromString<QVersionNumber>(_ui->kernelVersionLneEdit->text(), QVersionNumber(0, 0, 0)) > QVersionNumber(0, 0, 0);
}

//////////
//  Implementation helpers
SharedFolders KernelEditor::_sharedFolders()
{
    SharedFolders result;
    for (int i = 0; i < _ui->sharedFoldersListWidget->count(); i++)
    {
        auto chunks =
            _ui->sharedFoldersListWidget->item(i)->text().split(_SharedFoldersSeparator);
        Q_ASSERT(chunks.size() == 2);
        result.insert(SharedFolder(chunks[0], chunks[1]));
    }
    return result;
}

void KernelEditor::_setSharedFolders(const SharedFolders & sharedFolders)
{
    //  Sort by volume name...
    QList<SharedFolder> sharedFoldersList = sharedFolders.values();
    std::sort(
        sharedFoldersList.begin(),
        sharedFoldersList.end(),
        [](const auto & a, const auto & b)
        {
            return a < b;
        });
    //  ...and replace list content
    _ui->sharedFoldersListWidget->clear();
    QSet<QString> volumeNames;
    for (auto sharedFolder : sharedFoldersList)
    {
        if (!volumeNames.contains(sharedFolder.volumeName()))
        {
            volumeNames.insert(sharedFolder.volumeName());
            _ui->sharedFoldersListWidget->addItem(
                sharedFolder.volumeName() +
                _SharedFoldersSeparator +
                sharedFolder.hostPath());
        }
    }
}

void KernelEditor::_addSharedFolder(const SharedFolder & sharedFolder)
{
    auto sharedFolders = _sharedFolders();
    for (auto sf : sharedFolders.values())
    {
        if (sf.volumeName() == sharedFolder.volumeName())
        {
            sharedFolders.remove(sf);
        }
    }
    sharedFolders.insert(sharedFolder);
    _setSharedFolders(sharedFolders);
}

void KernelEditor::_removeSharedFolder(const QString & volumeName)
{
    auto sharedFolders = _sharedFolders();
    for (auto sf : sharedFolders.values())
    {
        if (sf.volumeName() == volumeName)
        {
            sharedFolders.remove(sf);
        }
    }
    _setSharedFolders(sharedFolders);
}

//////////
//  Signal handlers
void KernelEditor::_kernelVersionLineEditTextChanged(QString)
{
    emit valueChanged();
}

void KernelEditor::_addSharedFolderPushButtonClicked()
{
    EditSharedFolderDialog dlg(this);
    if (dlg.doModal() == EditSharedFolderDialog::Result::Ok)
    {
        _addSharedFolder(dlg.sharedFolder());
    }
}

void KernelEditor::_modifySharedFolderPushButtonClicked()
{
    QMessageBox::critical(this, "ERROR", "Not yet implemented");
}

void KernelEditor::_removeSharedFolderPushButtonClicked()
{
    QMessageBox::critical(this, "ERROR", "Not yet implemented");
}

//  End of emuone-hades/KernelEditor.cpp
