//
//  emuone-hadesos/HadesOsEditor.cpp
//
//  hadesos::HadesOsEditor class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_HadesOsEditor.h"
#pragma GCC diagnostic pop
using namespace hadesos;

//////////
//  Construction/destruction
HadesOsEditor::HadesOsEditor(HadesOs * hadesOs, QWidget * parent)
    :   core::ComponentEditor(hadesOs, parent),
        _ui(new Ui::HadesOsEditor),
        _hadesOs(hadesOs)
{
    _ui->setupUi(this);
}

HadesOsEditor::~HadesOsEditor()
{
    delete _ui;
}

//////////
//  core::ComponentEditor
void HadesOsEditor::refresh()
{
    _refreshUnderway = true;

    _refreshSharedHostFoldersList();
    _refreshActionButtons();

    _refreshUnderway = false;
}

//////////
//  Implementation helpers
void HadesOsEditor::_refreshSharedHostFoldersList()
{
    HadesOs::SharedHostFolderList sharedHostFolders = _hadesOs->sharedHostFolders();
    std::sort(sharedHostFolders.begin(), sharedHostFolders.end(),
              [](auto a, auto b) { return a.volumeName().toLower() < b.volumeName().toLower(); });
    //  Make sure the "shared host folders" list has a proper number of items...
    while (_ui->_sharedHostFoldersListWidget->count() > sharedHostFolders.size())
    {   //  OOPS! Too many items in the UI list
        delete _ui->_sharedHostFoldersListWidget->takeItem(_ui->_sharedHostFoldersListWidget->count());
    }
    while (_ui->_sharedHostFoldersListWidget->count() < sharedHostFolders.size())
    {   //  OOPS! Too few items in the UI list
        _ui->_sharedHostFoldersListWidget->addItem("");
    }
    //  ...with correct properties
    for (int i = 0; i < sharedHostFolders.size(); i++)
    {
        QListWidgetItem * item = _ui->_sharedHostFoldersListWidget->item(i);
        if (item->text() != sharedHostFolders[i].toString())
        {
            item->setText(sharedHostFolders[i].toString());
        }
    }
}

void HadesOsEditor::_refreshActionButtons()
{
    HadesOs::SharedHostFolder selectedSharedHostFolder = _selectedSharedHostFolder();
    _ui->_modifySharedHostFolderPushButton->setEnabled(selectedSharedHostFolder.isValid());
    _ui->_removeSharedHostFolderPushButton->setEnabled(selectedSharedHostFolder.isValid());
}

HadesOs::SharedHostFolder HadesOsEditor::_selectedSharedHostFolder()
{
    HadesOs::SharedHostFolderList sharedHostFolders = _hadesOs->sharedHostFolders();
    if (QListWidgetItem * item = _ui->_sharedHostFoldersListWidget->currentItem())
    {   //  There IS a "current" item
        QString currentItemText = item->text();
        for (HadesOs::SharedHostFolder sharedHostFolder : sharedHostFolders)
        {
            if (sharedHostFolder.toString() == currentItemText)
            {
                return sharedHostFolder;
            }
        }
    }
    return HadesOs::SharedHostFolder();
}

void HadesOsEditor::_addSharedHostFolder()
{
    AddSharedHostFolderDialog dlg(this->topLevelWidget());
    if (dlg.exec() == QDialog::DialogCode::Accepted)
    {   //  Add
    }
}

void HadesOsEditor::_modifySharedHostFolder()
{
}

void HadesOsEditor::_removeSharedHostFolder()
{
}

//////////
//  Signal handlers
void HadesOsEditor::_addSharedHostFolderPushButtonClicked()
{
    _addSharedHostFolder();
}

void HadesOsEditor::_modifySharedHostFolderPushButtonClicked()
{
    _modifySharedHostFolder();
}

void HadesOsEditor::_removeSharedHostFolderPushButtonClicked()
{
    _removeSharedHostFolder();
}

void HadesOsEditor::_sharedHostFoldersListWidgetCurrentRowChanged(int)
{
}

//  End of emuone-hadesos/HadesOsEditor.cpp
