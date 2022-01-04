//
//  emuone/ConfigureVmDialog.cpp
//
//  "Configure VM" dialog
//
//////////
#include "emuone/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_ConfigureVmDialog.h"
#pragma GCC diagnostic pop

//////////
//  Construction/destruction
ConfigureVmDialog::ConfigureVmDialog(core::VirtualAppliance * virtualAppliance, QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ConfigureVmDialog),
        _virtualAppliance(virtualAppliance),
        _componentEditorsPanelLayout(nullptr)
{
    Q_ASSERT(_virtualAppliance != nullptr);

    _ui->setupUi(this);
    _componentEditorsPanelLayout = new QGridLayout(_ui->_componentEditorsPanel);
    _componentEditorsPanelLayout->setContentsMargins(0, 0, 0, 0);
    _ui->_componentEditorsPanel->setLayout(_componentEditorsPanelLayout);

    _ui->_nameLineEdit->setText(virtualAppliance->name());
    _ui->_locationLineEdit->setText(virtualAppliance->location());
    _ui->_typeLineEdit->setText(virtualAppliance->type()->displayName());
    _ui->_architectureLineEdit->setText(virtualAppliance->architecture()->displayName());
    _ui->_templateLineEdit->setText(virtualAppliance->initialTemplate()->displayName());

    //  Populate the "component editors" map for current VA components
    for (core::Component * component : virtualAppliance->components())
    {
        if (core::ComponentEditor * componentEditor = component->createEditor(_ui->_componentEditorsPanel))
        {
            componentEditor->setVisible(false);
            _componentEditors.insert(component, componentEditor);
            connect(componentEditor, &core::ComponentEditor::componentConfigurationChanged, this, &ConfigureVmDialog::_componentConfigurationChanged);
            _componentEditorsPanelLayout->addWidget(componentEditor, 0, 0);
        }
    }
    for (core::Adaptor * adaptor : virtualAppliance->adaptors())
    {
        if (core::AdaptorEditor * adaptorEditor = adaptor->createEditor(_ui->_componentEditorsPanel))
        {
            adaptorEditor->setVisible(false);
            _adaptorEditors.insert(adaptor, adaptorEditor);
            connect(adaptorEditor, &core::AdaptorEditor::adaptorConfigurationChanged, this, &ConfigureVmDialog::_adaptorConfigurationChanged);
            _componentEditorsPanelLayout->addWidget(adaptorEditor, 0, 0);
        }
    }

    //  Done
    _refreshComponentsTree();
    _refresh();

    _ui->_componentsTreeWidget->expandAll();

    //  Put the dialog in the screen center
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    this->move(screenGeometry.center() - this->rect().center());
}

ConfigureVmDialog::~ConfigureVmDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void ConfigureVmDialog::_refreshComponentsTree()
{
    core::ComponentCategoryList componentCategories = core::ComponentCategory::all();
    std::sort(componentCategories.begin(), componentCategories.end(),
              [](auto a, auto b) -> bool { return a->displayName() < b->displayName(); });
    //  Root nodes
    while (componentCategories.size() < _ui->_componentsTreeWidget->topLevelItemCount())
    {   //  Too many "top-level" items in the "components" tree
        delete _ui->_componentsTreeWidget->takeTopLevelItem(0);
    }
    while (componentCategories.size() > _ui->_componentsTreeWidget->topLevelItemCount())
    {   //  Too few "top-level" items in the "components" tree
        _ui->_componentsTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    for (int i = 0; i < componentCategories.size(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = _ui->_componentsTreeWidget->topLevelItem(i);
        componentCategoryItem->setText(0, componentCategories[i]->displayName());
        componentCategoryItem->setIcon(0, componentCategories[i]->smallIcon());
        componentCategoryItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(static_cast<void*>(componentCategories[i])));
        //  Now for the components
        core::ComponentList components = _virtualAppliance->components(componentCategories[i]);
        std::sort(components.begin(), components.end(),
                  [](auto a, auto b) -> bool { return a->name() < b->name(); });
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
            componentItem->setText(0, components[j]->name() + " " + components[j]->shortStatus() + " (" + components[j]->type()->displayName() + ")");
            componentItem->setIcon(0, components[j]->type()->smallIcon());
            componentItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(static_cast<void*>(components[j])));
        }
    }
}

void ConfigureVmDialog::_refresh()
{
    _refreshUnderway = true;
    core::Component * selectedComponent = _getSelectedComponent();

    //  Refresh "component name" line edit
    _ui->_componentNameLineEdit->setText((selectedComponent != nullptr) ? selectedComponent->name() : "");

    //  Refresh all "component editors", making sure only the right one is visible
    core::ComponentEditor * currentComponentEditor = nullptr;
    for (core::ComponentEditor * componentEditor : _componentEditors)
    {
        componentEditor->refresh();
        if (componentEditor->component() == selectedComponent)
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
    _ui->_propertiesGroupBox->setVisible(currentComponentEditor != nullptr);

    //  Done
    _refreshUnderway = false;
}

core::ComponentCategory * ConfigureVmDialog::_getSelectedComponentCategory()
{
    for (int i = 0; i < _ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = _ui->_componentsTreeWidget->topLevelItem(i);
        if (_ui->_componentsTreeWidget->currentItem() == componentCategoryItem)
        {
            return static_cast<core::ComponentCategory*>(componentCategoryItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
        }
    }
    return nullptr;
}

core::Component * ConfigureVmDialog::_getSelectedComponent()
{
    for (int i = 0; i < _ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = _ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(j);
            if (_ui->_componentsTreeWidget->currentItem() == componentItem)
            {
                return static_cast<core::Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
            }
        }
    }
    return nullptr;
}

void ConfigureVmDialog::_setSelectedComponent(core::Component * component)
{
    for (int i = 0; i < _ui->_componentsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * componentCategoryItem = _ui->_componentsTreeWidget->topLevelItem(i);
        for (int j = 0; j < componentCategoryItem->childCount(); j++)
        {
            QTreeWidgetItem * componentItem = componentCategoryItem->child(j);
            core::Component * itemComponent = static_cast<core::Component*>(componentItem->data(0, Qt::ItemDataRole::UserRole).value<void*>());
            if (itemComponent == component)
            {   //  This one!
                _ui->_componentsTreeWidget->setCurrentItem(componentItem);
                return;
            }
        }
    }
}

QMenu * ConfigureVmDialog::_createAddAnyComponentPopupMenu()
{
    QMenu * menu = new QMenu();

    core::ComponentCategoryList componentCategories = core::ComponentCategory::all();
    std::sort(componentCategories.begin(), componentCategories.end(),
              [](auto a, auto b) -> bool { return a->displayName() < b->displayName(); });
    for (core::ComponentCategory * componentCategory : componentCategories)
    {
        QMenu * componentCategoryMenu = menu->addMenu(componentCategory->smallIcon(), componentCategory->displayName());
        core::ComponentTypeList componentTypes = componentCategory->componentTypes();
        if (componentTypes.size() > 0)
        {
            std::sort(componentTypes.begin(), componentTypes.end(),
                      [](auto a, auto b) -> bool { return a->displayName() < b->displayName(); });
            for (core::ComponentType * componentType : componentTypes)
            {
                if (componentType->isCompatibleWith(_virtualAppliance->architecture()))  //  TODO or adaptable to it
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
        if (core::ComponentEditor * componentEditor = component->createEditor(_ui->_componentEditorsPanel))
        {
            componentEditor->setVisible(false);
            _componentEditors.insert(component, componentEditor);
            connect(componentEditor, &core::ComponentEditor::componentConfigurationChanged, this, &ConfigureVmDialog::_componentConfigurationChanged);
            _componentEditorsPanelLayout->addWidget(componentEditor, 0, 0);
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
        msgBox.setText(ex.message());
        msgBox.exec();
        return nullptr;
    }
}

void ConfigureVmDialog::_removeComponent(core::Component * component)
{
    //  Confirm
    QString message =
        "Are you sure you want to remove the " + component->type()->displayName() +
        " " + component->name() + "?";
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Remove component", message, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {   //  ...and remove
        _virtualAppliance->removeComponent(component);
        _refreshComponentsTree();
        //  Are there editor(s) involved ?
        if (_componentEditors.contains(component))
        {
            core::ComponentEditor * componentEditor = _componentEditors[component];
            _componentEditorsPanelLayout->removeWidget(componentEditor);
            _componentEditors.remove(component);
            delete componentEditor;
        }
        //  TODO if the "component" is "adapted", there may be an editor for the "adaptor"
        //  Update UI
        _refresh();
        delete component;
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
    popupMenuOrigin = _ui->_addComponentPushButton->mapToGlobal(popupMenuOrigin);
    popupMenuOrigin.setY(popupMenuOrigin.y() - popupMenuSizeHint.height());
    popupMenu->popup(popupMenuOrigin);
}

void ConfigureVmDialog::_removeComponentPushButtonClicked()
{
    if (core::Component * component = _getSelectedComponent())
    {
        _removeComponent(component);
    }
}

void ConfigureVmDialog::_componentNameLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway && core::Component::isValidName(_ui->_componentNameLineEdit->text()))
    {
        if (core::Component * selectedComponent = _getSelectedComponent())
        {
            selectedComponent->setName(_ui->_componentNameLineEdit->text());
            _refreshComponentsTree();
            _setSelectedComponent(selectedComponent);   //  because component name may have changed!
        }
    }
}

void ConfigureVmDialog::_componentConfigurationChanged(core::Component * component)
{
    _refreshComponentsTree();
    _setSelectedComponent(component);   //  because component "name" may have changed!
}

void ConfigureVmDialog::_adaptorConfigurationChanged(core::Adaptor * adaptor)
{
    _refreshComponentsTree();
    _setSelectedComponent(adaptor->adaptedComponent());   //  because component "name " may have changed!
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
        msgBox.setText(ex.message());
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
