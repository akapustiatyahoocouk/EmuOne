//
//  emuone/ConfigureVmDialog.cpp
//
//  "Configure VM" dialog
//
//////////
#include "emuone/API.hpp"

#include "ConfigureVmDialog.hpp"
#include "ui_ConfigureVmDialog.h"

ConfigureVmDialog::ConfigureVmDialog(VirtualAppliance * virtualAppliance, QWidget * parent)
    :   QDialog(parent),
        ui(new Ui::ConfigureVmDialog)
{
    _virtualAppliance = virtualAppliance;
    Q_ASSERT(_virtualAppliance != nullptr);

    ui->setupUi(this);

    ui->_nameLineEdit->setText(virtualAppliance->getName());
    ui->_locationLineEdit->setText(virtualAppliance->getLocation());
    ui->_typeLineEdit->setText(virtualAppliance->getType()->getDisplayName());
    ui->_architectureLineEdit->setText(virtualAppliance->getArchitecture()->getDisplayName());
    ui->_templateLineEdit->setText(virtualAppliance->getTemplate()->getDisplayName());

    //  Done
    _refreshComponentsTree();
    _refresh();
}

ConfigureVmDialog::~ConfigureVmDialog()
{
    delete ui;
}

//////////
//  Implementation helpers
void ConfigureVmDialog::_refreshComponentsTree()
{
    ComponentCategoryList componentCategories = ComponentCategory::getAll();
    std::sort(componentCategories.begin(), componentCategories.end(),
              [](auto a, auto b) -> bool { return a->getDisplayName() < b->getDisplayName(); });
    //  Root nodes
    while (componentCategories.size() < ui->_componentsTreeWidget->topLevelItemCount())
    {   //  Too many "top-level" items in the "components" tree
        delete ui->_componentsTreeWidget->takeTopLevelItem(0);
    }
    while (componentCategories.size() > ui->_componentsTreeWidget->topLevelItemCount())
    {   //  Too few "top-level" items in the "components" tree
        ui->_componentsTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    for (int i = 0; i < componentCategories.size(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        componentCategoryItem->setText(0, componentCategories[i]->getDisplayName());
        componentCategoryItem->setIcon(0, componentCategories[i]->getSmallIcon());
        componentCategoryItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(static_cast<void*>(componentCategories[i])));
        //  Now for the components
        ComponentList components = _virtualAppliance->getComponents(componentCategories[i]);
        std::sort(components.begin(), components.end(),
                  [](auto a, auto b) -> bool { return a->getName() < b->getName(); });
        while (components.size() < componentCategoryItem->childCount())
        {   //  Too many "2nd level" items in the "components" tree
            delete componentCategoryItem->takeChild(0);
        }
        while (components.size() > componentCategoryItem->childCount())
        {   //  Too few "2nd level" items in the "components" tree
            componentCategoryItem->addChild(new QTreeWidgetItem());
        }
        for (int i = 0; i < components.size(); i++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(i);
            componentItem->setText(0, components[i]->getName() + " (" + components[i]->getType()->getDisplayName() + ")");
            componentItem->setIcon(0, components[i]->getType()->getSmallIcon());
            componentItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(static_cast<void*>(components[i])));
        }
    }
}

void ConfigureVmDialog::_refresh()
{
}

ComponentCategory * ConfigureVmDialog::_getSelectedComponentCategory()
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        if (ui->_componentsTreeWidget->currentItem() == componentCategoryItem)
        {
            return static_cast<ComponentCategory*>(componentCategoryItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
        }
    }
    return nullptr;
}

Component * ConfigureVmDialog::_getSelectedComponent()
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(i);
            if (ui->_componentsTreeWidget->currentItem() == componentItem)
            {
                return static_cast<Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
            }
        }
    }
    return nullptr;
}

void ConfigureVmDialog::_setSelectedComponent(Component * component)
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(i);
            Component * itemComponent = static_cast<Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
            if (itemComponent == component)
            {   //  This one!
                ui->_componentsTreeWidget->setCurrentItem(componentItem);
                return;
            }
        }
    }
}

QMenu * ConfigureVmDialog::_createAddAnyComponentPopupMenu()
{
    QMenu * menu = new QMenu();

    ComponentCategoryList componentCategories = ComponentCategory::getAll();
    std::sort(componentCategories.begin(), componentCategories.end(),
              [](auto a, auto b) -> bool { return a->getDisplayName() < b->getDisplayName(); });
    for (ComponentCategory * componentCategory : componentCategories)
    {
        QMenu * componentCategoryMenu = menu->addMenu(componentCategory->getSmallIcon(), componentCategory->getDisplayName());
        ComponentTypeList componentTypes = componentCategory->getComponentTypes();
        if (componentTypes.size() > 0)
        {
            std::sort(componentTypes.begin(), componentTypes.end(),
                      [](auto a, auto b) -> bool { return a->getDisplayName() < b->getDisplayName(); });
            for (ComponentType * componentType : componentTypes)
            {
                if (componentType->isCompatibleWith(_virtualAppliance->getArchitecture()))  //  TODO or adaptable to it
                {
                    _AddComponentAction * action = new _AddComponentAction(this, componentType);
                    connect(action, &QAction::triggered, action, &_AddComponentAction::_triggered);
                    componentCategoryMenu->addAction(action);
                }
            }
        }
        else
        {   //  No applicable component types in this category
            componentCategoryMenu->setEnabled(false);
        }
    }
    return menu;
}

Component * ConfigureVmDialog::_addComponent(ComponentType * componentType)
{
    componentType = componentType;
    return nullptr;
}

//////////
//  Event handlers
void ConfigureVmDialog::_addComponentPushButtonClicked()
{
    QMenu * popupMenu = _createAddAnyComponentPopupMenu();
    QSize popupMenuSizeHint = popupMenu->sizeHint();
    QPoint popupMenuOrigin(0, 0);
    popupMenuOrigin = ui->_addComponentPushButton->mapToGlobal(popupMenuOrigin);
    popupMenuOrigin.setY(popupMenuOrigin.y() - popupMenuSizeHint.height());
    popupMenu->popup(popupMenuOrigin);
}

void ConfigureVmDialog::_removeComponentPushButtonClicked()
{
}

void ConfigureVmDialog::_accept()
{
    try
    {
        _virtualAppliance->save();
        QDialog::accept();
    }
    catch (VirtualApplianceException & ex)
    {   //  OOPS! Report & abort
        QMessageBox msgBox;
        msgBox.setText(ex.getMessage());
        msgBox.exec();
        return;
    }
}

void ConfigureVmDialog::_reject()
{
    //  TODO restore VA configuration
    QDialog::reject();
}

//  End of emuone/ConfigureVmDialog.cpp
