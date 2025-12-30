//
//  emuone-hades/devices/ExternalFileSystemEditor.cpp - emuone::hades::devices::ExternalFileSystemEditor class implementation
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
#include "ui_ExternalFileSystemEditor.h"
using namespace emuone::hades::devices;

//////////
//  Construction/destruction
ExternalFileSystemEditor::ExternalFileSystemEditor(
        QWidget * parent,
        ExternalFileSystem * externalFileSystem
    ) : emuone::core::ComponentEditor(parent, externalFileSystem),
        _externalFileSystem(externalFileSystem),
        _ui(new Ui::ExternalFileSystemEditor)
{
    _ui->setupUi(this);
}

ExternalFileSystemEditor::~ExternalFileSystemEditor()
{
    delete _ui;
}

//////////
//  emuone::core::ComponentEditor
void ExternalFileSystemEditor::loadControlValues()
{
    _ui->volumeNameLineEdit->setText(_externalFileSystem->volumeName());
    _ui->hostPathLineEdit->setText(_externalFileSystem->hostPath());
}

void ExternalFileSystemEditor::saveControlValues() const
{
    if (isValid())
    {
        _externalFileSystem->setVolumeName(_ui->volumeNameLineEdit->text());
        _externalFileSystem->setHostPath(_ui->hostPathLineEdit->text());
    }
}

bool ExternalFileSystemEditor::isValid() const
{
    return ExternalFileSystem::isValidVolumeName(_ui->volumeNameLineEdit->text()) &&
           ExternalFileSystem::isValidHostPath(_ui->hostPathLineEdit->text());
}

//////////
//  Signal handlers
void ExternalFileSystemEditor::_volumeNameLineEditTextChanged(QString)
{
    _externalFileSystem->setVolumeName(_ui->volumeNameLineEdit->text());
    emit valueChanged();
}

void ExternalFileSystemEditor::_hostPathLineEditTextChanged(QString)
{
    _externalFileSystem->setHostPath(_ui->hostPathLineEdit->text());
    emit valueChanged();
}

void ExternalFileSystemEditor::_browsePushButtonClicked()
{
    QString dir =
        QFileDialog::getExistingDirectory(
            this,
            "Select host folder",
            _externalFileSystem->owner()->toAbsolutePath(
                _ui->hostPathLineEdit->text()),
            QFileDialog::ShowDirsOnly |
                QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        _ui->hostPathLineEdit->setText(
            _externalFileSystem->owner()->toRelatimePath(dir));
        emit valueChanged();
    }
}

//  End of emuone-hades/devices/ExternalFileSystemEditor.cpp
