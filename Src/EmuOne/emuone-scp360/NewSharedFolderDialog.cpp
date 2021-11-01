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
}

NewSharedFolderDialog::~NewSharedFolderDialog()
{
    delete _ui;
}

//////////
//  Event handlers
void NewSharedFolderDialog::_accept()
{
    accept();
}

void NewSharedFolderDialog::_reject()
{
    reject();
}

//  End of emuone-scp360/NewSharedFolderDialog.cpp
