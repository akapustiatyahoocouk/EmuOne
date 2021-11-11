//
//  emuone/ConfigureVmDialog.hpp
//
//  "Configure VM" dialog
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QDialog>

namespace Ui { class ConfigureVmDialog; }

//////////
//  The "Configure VM" dialog
class ConfigureVmDialog final : public QDialog
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfigureVmDialog)

    //////////
    //  Construction/destruction
public:
    ConfigureVmDialog(core::VirtualAppliance * virtualAppliance, QWidget * parent = nullptr);
    ~ConfigureVmDialog();

    //////////
    //  Implementation
private:
    Ui::ConfigureVmDialog * _ui;
    core::VirtualAppliance *_virtualAppliance;
    bool                    _refreshUnderway = false;
    QGridLayout *           _componentEditorsPanelLayout;

    QMap<core::Component*, core::ComponentEditor*>  _componentEditors = {};

    class _AddComponentAction : public QAction
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_AddComponentAction)

    public:
        _AddComponentAction(ConfigureVmDialog * configureVmDialog, core::ComponentType * componentType)
            :   QAction(componentType->smallIcon(), componentType->displayName()),
                _configureVmDialog(configureVmDialog),
                _componentType(componentType)
        {
        }

        ConfigureVmDialog *const    _configureVmDialog;
        core::ComponentType *const  _componentType;

    public:
        void                _triggered() { _configureVmDialog->_addComponent(_componentType); }
    };

    //  Helpers
    void                    _refreshComponentsTree();
    void                    _refresh();

    core::ComponentCategory*_getSelectedComponentCategory();
    core::Component *       _getSelectedComponent();
    void                    _setSelectedComponent(core::Component * component);

    QMenu *                 _createAddAnyComponentPopupMenu();

    core::Component *       _addComponent(core::ComponentType * componentType);
    void                    _removeComponent(core::Component * component);

    //////////
    //  Event handlers
private slots:
    void                    _componentsTreeWidgetItemSelectionChanged();
    void                    _addComponentPushButtonClicked();
    void                    _removeComponentPushButtonClicked();
    void                    _componentNameLineEditTextChanged(const QString &);
    void                    _componentConfigurationChanged(core::Component * component);
    void                    _accept();
    void                    _reject();
};

//  End of emuone/ConfigureVmDialog.hpp
