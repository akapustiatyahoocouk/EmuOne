//
//  emuone-hadesos/AddSharedHostFolderDialog.cpp
//
//  hadesos::AddSharedHostFolderDialog class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_AddSharedHostFolderDialog.h"
#pragma GCC diagnostic pop
using namespace hadesos;

//////////
//  Construction/destruction
AddSharedHostFolderDialog::AddSharedHostFolderDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::AddSharedHostFolderDialog)
{
    _ui->setupUi(this);

    _refreshActionButons();
}

AddSharedHostFolderDialog::~AddSharedHostFolderDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void AddSharedHostFolderDialog::_refreshActionButons()
{
    _ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                _ui->_volumeNameLineEdit->text().trimmed().length() > 0 &&
                _ui->_hostPathLineEdit->text().trimmed().length() > 0 &&
                Validation::isValidVolumeName(_ui->_volumeNameLineEdit->text().trimmed())); //  TODO and "host path" too
}

//////////
//  Event handlers
void AddSharedHostFolderDialog::_volumeNameLineEditTextChanged(const QString &)
{
    _refreshActionButons();
}

void AddSharedHostFolderDialog::_hostPathLineEditTextChanged(const QString &)
{
    _refreshActionButons();
}

void AddSharedHostFolderDialog::_browsePushButtonClicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Select volume host path");
    if (!dirPath.isEmpty())
    {
        _ui->_hostPathLineEdit->setText(dirPath);
    }
}

void AddSharedHostFolderDialog::_accept()
{
    _sharedHostFolderVolumeName = _ui->_volumeNameLineEdit->text().trimmed();
    _sharedHostFolderHostPath = _ui->_hostPathLineEdit->text().trimmed();
    accept();
}

void AddSharedHostFolderDialog::_reject()
{
    reject();
}

//  End of emuone-hadesos/AddSharedHostFolderDialog.cpp
