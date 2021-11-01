//
//  emuone-scp360/ScpEditor.cpp
//
//  ScpEditor class implementation
//
//////////
#include "emuone-scp360/API.hpp"
#include "ui_ScpEditor.h"
using namespace scp360;

//////////
//  Construction/destruction
ScpEditor::ScpEditor(scp360::Scp * scp, QWidget * parent)
    :   ComponentEditor(scp, parent),
        _ui(new Ui::ScpEditor),
        _scp(scp)
{
    _ui->setupUi(this);
    Q_ASSERT(_scp != nullptr);

    _ui->_sharedFoldersTreeWidget->headerItem()->setText(0, "Volume");
    _ui->_sharedFoldersTreeWidget->headerItem()->setText(1, "Host path");
}

ScpEditor::~ScpEditor()
{
    _scp->_editors.removeOne(this);
    delete _ui;
}

//////////
//  ComponentEditor
void ScpEditor::refresh()
{
    _refreshUnderway = true;

    //  TODO

    _refreshUnderway = false;
}

//////////
//  Event handlers
void ScpEditor::_addSharedFolderPushButtonClicked()
{
    NewSharedFolderDialog dlg(this->topLevelWidget());
    if (dlg.exec() == QDialog::DialogCode::Accepted)
    {
    }
}

void ScpEditor::_removeSharedFolderPushButtonClicked()
{
}

void ScpEditor::_modifySharedFolderPushButtonClicked()
{
}

//  End of emuone-scp360/ScpEditor.cpp

