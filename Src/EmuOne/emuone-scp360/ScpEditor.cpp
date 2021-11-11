//
//  emuone-scp360/ScpEditor.cpp
//
//  ScpEditor class implementation
//
//////////
#include "emuone-scp360/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_ScpEditor.h"
#pragma GCC diagnostic pop
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

    _refreshSharedFoldersList();
    refresh();
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

    Scp::SharedFolder * sharedFolder = _selectedSharedFolder();
    _ui->_removeSharedFolderPushButton->setEnabled(sharedFolder != nullptr);
    _ui->_modifySharedFolderPushButton->setEnabled(sharedFolder != nullptr);

    _refreshUnderway = false;
}

//////////
//  Implementation helpers
void ScpEditor::_refreshSharedFoldersList()
{
    QList<scp360::Scp::SharedFolder*> sharedFolders = _scp->sharedFolders();
    std::sort(sharedFolders.begin(), sharedFolders.end(),
              [](auto a, auto b) { return a->volumeName() < b->volumeName(); });
    //  Make sure the "shared folders" tree has a proper number of "root" nodes...
    while (_ui->_sharedFoldersTreeWidget->topLevelItemCount() > sharedFolders.size())
    {   //  Too many items in the tree
        delete _ui->_sharedFoldersTreeWidget->takeTopLevelItem(0);
    }
    while (_ui->_sharedFoldersTreeWidget->topLevelItemCount() < sharedFolders.size())
    {   //  Too few items in the tree
        _ui->_sharedFoldersTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    //  ...with proper properties
    for (int i = 0; i < sharedFolders.size(); i++)
    {
        scp360::Scp::SharedFolder * sharedFolder = sharedFolders[i];
        QTreeWidgetItem * treeItem = _ui->_sharedFoldersTreeWidget->topLevelItem(i);
        treeItem->setText(0, sharedFolder->volumeName());
        treeItem->setText(1, sharedFolder->hostPath());
        treeItem->setData(2, 0, QVariant::fromValue(static_cast<void*>(sharedFolder)));
    }
}

scp360::Scp::SharedFolder * ScpEditor::_selectedSharedFolder()
{
    if (QTreeWidgetItem * treeItem = _ui->_sharedFoldersTreeWidget->currentItem())
    {
        return static_cast<scp360::Scp::SharedFolder*>(treeItem->data(2, 0).value<void*>());
    }
    return nullptr;
}

void ScpEditor::_setSelectedSharedFolder(scp360::Scp::SharedFolder * sharedFolder)
{
    for (int i = 0; i < _ui->_sharedFoldersTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * treeItem = _ui->_sharedFoldersTreeWidget->topLevelItem(i);
        if (static_cast<scp360::Scp::SharedFolder*>(treeItem->data(2, 0).value<void*>()) == sharedFolder)
        {   //  This one!
            _ui->_sharedFoldersTreeWidget->setCurrentItem(treeItem);
            break;
        }
    }
}

//////////
//  Event handlers
void ScpEditor::_sharedFoldersTreeWidgetItemSelectionChanged()
{
    refresh();
}

void ScpEditor::_addSharedFolderPushButtonClicked()
{
    NewSharedFolderDialog dlg(this->topLevelWidget());
    if (dlg.exec() == QDialog::DialogCode::Accepted)
    {   //  Prepare new shared folder properties
        QString volumeName = dlg.volumeName();
        QString hostPath = dlg.hostPath();
        if (_scp->virtualAppliance() != nullptr)
        {   //  Try to use a relative "host path"
            QDir vaDir(QFileInfo(_scp->virtualAppliance()->location()).absoluteDir());
            QString relativeHostPath = vaDir.relativeFilePath(hostPath);
            if (!relativeHostPath.contains(".."))
            {   //  Good to go...
                hostPath = relativeHostPath;
                //  ...but we want it to look like a path still
                if (QFileInfo(hostPath).fileName() == hostPath)
                {   //  ...so prepend with "./"
                    hostPath = "./" + hostPath;
                }
            }
        }
        //  Create/modify the shared folder
        Scp::SharedFolder * sharedFolder = _scp->createSharedFolder(volumeName, hostPath);
        _refreshSharedFoldersList();
        refresh();
        _setSelectedSharedFolder(sharedFolder);
    }
}

void ScpEditor::_removeSharedFolderPushButtonClicked()
{
    if (Scp::SharedFolder * sharedFolder = _selectedSharedFolder())
    {

        QString prompt = "Are you sure you want to remove the shared folder " +
                         _scp->virtualAppliance()->toAbsolutePath(sharedFolder->hostPath()) +
                         " mounted as " + sharedFolder->volumeName() + " ?";
        if (QMessageBox::question(this, "Remove shared folder", prompt) == QMessageBox::StandardButton::Yes)
        {   //  Remove & refresh
            _scp->destroySharedFolder(sharedFolder->volumeName());
            _refreshSharedFoldersList();
            refresh();
        }
    }
}

void ScpEditor::_modifySharedFolderPushButtonClicked()
{
    if (Scp::SharedFolder * sharedFolder = _selectedSharedFolder())
    {
        ModifySharedFolderDialog dlg(sharedFolder, this);
        if (dlg.exec() == QDialog::DialogCode::Accepted)
        {   //  Update UI
            _refreshSharedFoldersList();
            refresh();
        }
    }
}

//  End of emuone-scp360/ScpEditor.cpp
