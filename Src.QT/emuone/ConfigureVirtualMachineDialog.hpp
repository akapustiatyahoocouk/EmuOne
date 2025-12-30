//
//  emuone/ConfigureVirtualMachineDialog.hpp - The modal "Configure VM" dialog
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#pragma once
#include "emuone/API.hpp"

namespace emuone
{
    namespace Ui { class ConfigureVirtualMachineDialog; }

    /// \class ConfigureVirtualMachineDialog emuone/API.hpp
    /// \brief The modal "Configure VM" dialog.
    class ConfigureVirtualMachineDialog final
        :   private QDialog
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfigureVirtualMachineDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< User's choice confirmed, VM configured.
            Cancel  ///< User has cancelled the VM creation.
        };

        //////////
        //  Construction/destruction
    public:
        ConfigureVirtualMachineDialog(
                QWidget * parent,
                emuone::core::VirtualMachine * virtualMachine
            );
        virtual ~ConfigureVirtualMachineDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Invoks the dialog modally.
        /// \return
        ///     The dialog result.
        Result          doModal();

        //////////
        //  Implementation
    private:
        emuone::core::VirtualMachine *const _virtualMachine;
        bool            _refreshUnderway = false;

        //  Data needed for rollback on cancel
        emuone::core::Components    _addedComponents;   //  to be removed on rollback
        emuone::core::Components    _removedComponents; //  to be restored on rollback
        QMap<emuone::core::IComponent*, QString>    _savedComponentConfigurations;
        QMap<emuone::core::IComponent*, QString>    _savedComponentAdaptorConfigurations;   //  for adapted comonents only
        QMap<emuone::core::IComponent*, emuone::core::IComponentAdaptorType*>    _preferredAdaptorTypes;    //  for adapted comonents only

        //  Helpers
        void            _refresh();
        void            _refreshComponentsTree();
        void            _refreshComponentCategoryItem(QTreeWidgetItem * categoryItem);
        void            _refreshComponentItem(QTreeWidgetItem * componentItem);
        QString         _saveConfiguration(
                                emuone::core::IComponent * component
                            );
        QString         _saveConfiguration(
                                emuone::core::IComponentAdaptor * componentAdaptor
                            );
        void            _restoreConfiguration(
                                emuone::core::IComponent * component,
                                const QString & configurationXml
                            );
        void            _restoreConfiguration(
                                emuone::core::IComponentAdaptor * componentAdaptor,
                                const QString & configurationXml
                            );
        QMenu *         _createAddComponentMenu();
        QMenu *         _createAddComponentMenu(emuone::core::IComponentCategory * componentCategory);
        QAction *       _createAddComponenyAction(emuone::core::IComponentType * componentType);

        auto            _selectedComponent() const -> emuone::core::IComponent *;
        void            _setSelectedComponent(emuone::core::IComponent * component);

        //////////
        //  Controls
    private:
        Ui::ConfigureVirtualMachineDialog * _ui;
        //  Dynamic controls
        QStackedLayout *    _editorsPanelLayout;
        QLabel *            _noPropertiesLabel;
        std::unique_ptr<QMenu>  _addComponentMenu;

        //////////
        //  Signal handlers
    private slots:
        void            _addComponentPushButtonClicked();
        void            _removeComponentPushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of emuone/ConfigureVirtualMachineDialog.hpp

