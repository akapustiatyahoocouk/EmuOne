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
class ConfigureVmDialog : public QDialog
{
    Q_OBJECT

    //////////
    //  Construction/destruction
public:
    ConfigureVmDialog(core::VirtualAppliance * virtualAppliance, QWidget * parent = nullptr);
    ~ConfigureVmDialog();

    //////////
    //  Implementation
private:
    Ui::ConfigureVmDialog * ui;
    core::VirtualAppliance *_virtualAppliance;
    bool                    _refreshUnderway = false;

    QMap<core::Component*, ComponentEditor*>    _componentEditors;

    class _AddComponentAction : public QAction
    {
    public:
        _AddComponentAction(ConfigureVmDialog * configureVmDialog, core::ComponentType * componentType)
            :   QAction(componentType->getSmallIcon(), componentType->getDisplayName()),
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

    core::ComponentCategory *   _getSelectedComponentCategory();
    core::Component *       _getSelectedComponent();
    void                    _setSelectedComponent(core::Component * component);

    QMenu *                 _createAddAnyComponentPopupMenu();

    core::Component *       _addComponent(core::ComponentType * componentType);

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
