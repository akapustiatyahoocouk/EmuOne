//
//  emuone/ConfigureVmDialog.cpp
//
//  "Configure VM" dialog
//
//////////
#include "emuone/API.hpp"

#include "ConfigureVmDialog.hpp"
#include "ui_ConfigureVmDialog.h"

ConfigureVmDialog::ConfigureVmDialog(core::VirtualAppliance * virtualAppliance, QWidget * parent)
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

    //  Populate the "component editors" map for current VA components
    for (core::Component * component : virtualAppliance->getComponents())
    {
        if (ComponentEditor * componentEditor = component->createEditor(ui->_componentEditorsScrollArea))
        {
            componentEditor->setVisible(false);
            _componentEditors.insert(component, componentEditor);
            connect(componentEditor, &ComponentEditor::componentConfigurationChanged, this, &ConfigureVmDialog::_componentConfigurationChanged);
        }
    }
    for (core::Adaptor * adaptor : virtualAppliance->getAdaptors())
    {
        if (ComponentEditor * componentEditor = adaptor->createEditor(ui->_componentEditorsScrollArea))
        {
            componentEditor->setVisible(false);
            _componentEditors.insert(adaptor, componentEditor);
            connect(componentEditor, &ComponentEditor::componentConfigurationChanged, this, &ConfigureVmDialog::_componentConfigurationChanged);
        }
    }

    //  Done
    _refreshComponentsTree();
    _refresh();

    ui->_componentsTreeWidget->expandAll();
}

ConfigureVmDialog::~ConfigureVmDialog()
{
    delete ui;
}

//////////
//  Implementation helpers
void ConfigureVmDialog::_refreshComponentsTree()
{
    core::ComponentCategoryList componentCategories = core::ComponentCategory::getAll();
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
        core::ComponentList components = _virtualAppliance->getComponents(componentCategories[i]);
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
        for (int j = 0; j < components.size(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(j);
            componentItem->setText(0, components[j]->getName() + " " + components[j]->getShortStatus() + " (" + components[j]->getType()->getDisplayName() + ")");
            componentItem->setIcon(0, components[j]->getType()->getSmallIcon());
            componentItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(static_cast<void*>(components[j])));
        }
    }
}

void ConfigureVmDialog::_refresh()
{
    _refreshUnderway = true;
    core::Component * selectedComponent = _getSelectedComponent();

    //  Refresh "component name" line edit
    ui->_componentNameLineEdit->setText((selectedComponent != nullptr) ? selectedComponent->getName() : "");

    //  Refresh all "component editors", making sure only the right one is visible
    ComponentEditor * currentComponentEditor = nullptr;
    for (ComponentEditor * componentEditor : _componentEditors)
    {
        componentEditor->refresh();
        if (componentEditor->getComponent() == selectedComponent)
        {
            componentEditor->setVisible(true);
            currentComponentEditor = componentEditor;
        }
        else
        {
            componentEditor->setVisible(false);
        }
    }

    //  Make sure proper "properties" controls are visible
    ui->_propertiesGroupBox->setVisible(currentComponentEditor != nullptr);

    //  Done
    _refreshUnderway = false;
}

core::ComponentCategory * ConfigureVmDialog::_getSelectedComponentCategory()
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        if (ui->_componentsTreeWidget->currentItem() == componentCategoryItem)
        {
            return static_cast<core::ComponentCategory*>(componentCategoryItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
        }
    }
    return nullptr;
}

core::Component * ConfigureVmDialog::_getSelectedComponent()
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(j);
            if (ui->_componentsTreeWidget->currentItem() == componentItem)
            {
                return static_cast<core::Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
            }
        }
    }
    return nullptr;
}

void ConfigureVmDialog::_setSelectedComponent(core::Component * component)
{
    for (int i = 0; i < ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(j);
            core::Component * itemComponent = static_cast<core::Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
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

    core::ComponentCategoryList componentCategories = core::ComponentCategory::getAll();
    std::sort(componentCategories.begin(), componentCategories.end(),
              [](auto a, auto b) -> bool { return a->getDisplayName() < b->getDisplayName(); });
    for (core::ComponentCategory * componentCategory : componentCategories)
    {
        QMenu * componentCategoryMenu = menu->addMenu(componentCategory->getSmallIcon(), componentCategory->getDisplayName());
        core::ComponentTypeList componentTypes = componentCategory->getComponentTypes();
        if (componentTypes.size() > 0)
        {
            std::sort(componentTypes.begin(), componentTypes.end(),
                      [](auto a, auto b) -> bool { return a->getDisplayName() < b->getDisplayName(); });
            for (core::ComponentType * componentType : componentTypes)
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

core::Component * ConfigureVmDialog::_addComponent(core::ComponentType * componentType)
{
    try
    {
        core::Component * component = componentType->createComponent();
        _virtualAppliance->addComponent(component);
        _refreshComponentsTree();
        //  Are there editor(s) involved ?
        if (ComponentEditor * componentEditor = component->createEditor(ui->_componentEditorsScrollArea))
        {
            componentEditor->setVisible(false);
            _componentEditors.insert(component, componentEditor);
            connect(componentEditor, &ComponentEditor::componentConfigurationChanged, this, &ConfigureVmDialog::_componentConfigurationChanged);
        }
        //  TODO if the "component" is "adapted", there may be an editor for the "adaptor"
        //  Update UI
        _setSelectedComponent(component);
        _refresh();
        return component;
    }
    catch (core::VirtualApplianceException & ex)
    {   //  OOPS! Report & abort
        QMessageBox msgBox;
        msgBox.setText(ex.getMessage());
        msgBox.exec();
        return nullptr;
    }
}

//////////
//  Event handlers
void ConfigureVmDialog::_componentsTreeWidgetItemSelectionChanged()
{
    _refresh();
}

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

void ConfigureVmDialog::_componentNameLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway && core::Component::isValidName(ui->_componentNameLineEdit->text()))
    {
        if (core::Component * selectedComponent = _getSelectedComponent())
        {
            selectedComponent->setName(ui->_componentNameLineEdit->text());
            _refreshComponentsTree();
            _setSelectedComponent(selectedComponent);   //  because component name may have changed!
        }
    }
}

void ConfigureVmDialog::_componentConfigurationChanged(core::Component * component)
{
    _refreshComponentsTree();
    _setSelectedComponent(component);   //  because component "name "short status" may have changed!
}

void ConfigureVmDialog::_accept()
{
    try
    {
        _virtualAppliance->save();
        QDialog::accept();
    }
    catch (core::VirtualApplianceException & ex)
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
