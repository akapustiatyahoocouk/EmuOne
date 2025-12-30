//
//  emuone-hades/EditSharedFolderDialog.cpp - emuone::hades::kernel::EditSharedFolderDialog class implementation
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
#include "ui_EditSharedFolderDialog.h"
using namespace emuone::hades::kernel;

//////////
//  Construction/destruction
EditSharedFolderDialog::EditSharedFolderDialog(
        QWidget * parent,
        const SharedFolder & sharedFolder
    ) : QDialog(parent),
        _sharedFolder(sharedFolder),
        _ui(new Ui::EditSharedFolderDialog)
{
    _ui->setupUi(this);

    //  Set static control values
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/emuone/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _ui->volumeNameLineEdit->setText(_sharedFolder.volumeName());
    _ui->hostPathLineEdit->setText(_sharedFolder.hostPath());

    //  Done
    _refresh();
    adjustSize();
    _ui->volumeNameLineEdit->setFocus();
}

EditSharedFolderDialog::~EditSharedFolderDialog()
{
    delete _ui;
}

//////////
//  Operations
auto EditSharedFolderDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void EditSharedFolderDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        SharedFolder::isValidVolumeName(_ui->volumeNameLineEdit->text()) &&
        SharedFolder::isValidHostPath(_ui->hostPathLineEdit->text()));
}

//////////
//  Signal handlers
void EditSharedFolderDialog::_volumeNameLineEditTextChanged(QString)
{
    _refresh();
}

void EditSharedFolderDialog::_hostPathLineEditTextChanged(QString)
{
    _refresh();
}

void EditSharedFolderDialog::_browsePushButtonClicked()
{
    QString dir =
        QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            _ui->hostPathLineEdit->text(),
            QFileDialog::ShowDirsOnly |
            QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        _ui->hostPathLineEdit->setText(dir);
    }
}

void EditSharedFolderDialog::accept()
{
    _sharedFolder = SharedFolder(
        _ui->volumeNameLineEdit->text(),
        _ui->hostPathLineEdit->text());
    done(int(Result::Ok));
}

void EditSharedFolderDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of emuone-hades/EditSharedFolderDialog.cpp
