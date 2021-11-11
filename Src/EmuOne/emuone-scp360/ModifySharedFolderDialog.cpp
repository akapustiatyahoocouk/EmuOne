//
//  emuone-scp360/ModifySharedFolderDialog.cpp
//
//  ModifySharedFolderDialog class implementation
//
//////////
#include "emuone-scp360/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_ModifySharedFolderDialog.h"
#pragma GCC diagnostic pop
using namespace scp360;

//////////
//  Construction/destruction
ModifySharedFolderDialog::ModifySharedFolderDialog(Scp::SharedFolder * sharedFolder, QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ModifySharedFolderDialog),
        _sharedFolder(sharedFolder)
{
    Q_ASSERT(_sharedFolder != nullptr);

    _ui->setupUi(this);

    _ui->_volumeNameLineEdit->setText(_sharedFolder->volumeName());
    _ui->_hostPathLineEdit->setText(_sharedFolder->scp()->virtualAppliance()->toAbsolutePath(_sharedFolder->hostPath()));

    //  Done
    _refresh();
}

ModifySharedFolderDialog::~ModifySharedFolderDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void ModifySharedFolderDialog::_refresh()
{
    QString volumeName = _ui->_volumeNameLineEdit->text().toUpper();
    QString hostPath = _ui->_hostPathLineEdit->text();

    _ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(Validator::isValidVolumeName(volumeName) &&
                                                              QFileInfo(hostPath).isDir());
}

//////////
//  Event handlers
void ModifySharedFolderDialog::_browsePushButtonClicked()
{
    QString hostPath = QFileDialog::getExistingDirectory(this, ("Select Host Folder"), QDir::currentPath());
    if (!hostPath.isEmpty())
    {
        _ui->_hostPathLineEdit->setText(hostPath);
        _refresh();
    }
}

void ModifySharedFolderDialog::_accept()
{
    _sharedFolder->setHostPath(_sharedFolder->scp()->virtualAppliance()->toRelativePath(_ui->_hostPathLineEdit->text()));
    accept();
}

void ModifySharedFolderDialog::_reject()
{
    reject();
}

//  End of ModifySharedFolderDialog class implementation
