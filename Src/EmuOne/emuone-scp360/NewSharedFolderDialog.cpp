//
//  emuone-scp360/NewSharedFolderDialog.cpp
//
//  NewSharedFolderDialog class implementation
//
//////////
#include "emuone-scp360/API.hpp"
#include "ui_NewSharedFolderDialog.h"
using namespace scp360;

//////////
//  Construction/destruction
NewSharedFolderDialog::NewSharedFolderDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::NewSharedFolderDialog)
{
    _ui->setupUi(this);

    _refresh();
}

NewSharedFolderDialog::~NewSharedFolderDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void NewSharedFolderDialog::_refresh()
{
    QString volumeName = _ui->_volumeNameLineEdit->text().toUpper();
    QString hostPath = _ui->_hostPathLineEdit->text();

    _ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(Validator::isValidVolumeName(volumeName) &&
                                                              QFileInfo(hostPath).isDir());
}

//////////
//  Event handlers
void NewSharedFolderDialog::_volumeNameLineEditTextChanged(const QString &)
{
    _refresh();
}

void NewSharedFolderDialog::_hostPathLineEditTextChanged(const QString &)
{
    _refresh();
}

void NewSharedFolderDialog::_browsePushButtonClicked()
{
    QString hostPath = QFileDialog::getExistingDirectory(this, ("Select Host Folder"), QDir::currentPath());
    if (!hostPath.isEmpty())
    {
        _ui->_hostPathLineEdit->setText(hostPath);
    }
}

void NewSharedFolderDialog::_accept()
{
    _volumeName = _ui->_volumeNameLineEdit->text().toUpper();
    _hostPath = _ui->_hostPathLineEdit->text();
    accept();
}

void NewSharedFolderDialog::_reject()
{
    _volumeName = _hostPath = "";
    reject();
}

//  End of emuone-scp360/NewSharedFolderDialog.cpp
