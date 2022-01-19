//
//  emuone-hadesos/ModifySharedHostFolderDialog.cpp
//
//  hadesos::ModifyAddSharedHostFolderDialog class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_ModifySharedHostFolderDialog.h"
#pragma GCC diagnostic pop
using namespace hadesos;

//////////
//  Construction/destruction
ModifySharedHostFolderDialog::ModifySharedHostFolderDialog(const HadesOs::SharedHostFolder & sharedHostFolder, QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ModifySharedHostFolderDialog),
        _originalSharedHostFolder(sharedHostFolder)
{
    _ui->setupUi(this);

    _ui->_volumeNameLineEdit->setText(sharedHostFolder.volumeName());
    _ui->_hostPathLineEdit->setText(sharedHostFolder.hostPath());
}

ModifySharedHostFolderDialog::~ModifySharedHostFolderDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void ModifySharedHostFolderDialog::_refreshActionButons()
{
    _ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                _ui->_volumeNameLineEdit->text().trimmed().length() > 0 &&
                _ui->_hostPathLineEdit->text().trimmed().length() > 0 &&
                Validation::isValidVolumeName(_ui->_volumeNameLineEdit->text().trimmed())); //  TODO and "host path" too
}

//////////
//  Event handlers
void ModifySharedHostFolderDialog::_volumeNameLineEditTextChanged(const QString &)
{
    _refreshActionButons();
}

void ModifySharedHostFolderDialog::_hostPathLineEditTextChanged(const QString &)
{
    _refreshActionButons();
}

void ModifySharedHostFolderDialog::_browsePushButtonClicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Select volume host path");
    if (!dirPath.isEmpty())
    {
        _ui->_hostPathLineEdit->setText(dirPath);
    }
}

void ModifySharedHostFolderDialog::_accept()
{
    _sharedHostFolderVolumeName = _ui->_volumeNameLineEdit->text().trimmed();
    _sharedHostFolderHostPath = _ui->_hostPathLineEdit->text().trimmed();
    accept();
}

void ModifySharedHostFolderDialog::_reject()
{
    reject();
}

//  End of emuone-hadesos/ModifySharedHostFolderDialog.cpp
