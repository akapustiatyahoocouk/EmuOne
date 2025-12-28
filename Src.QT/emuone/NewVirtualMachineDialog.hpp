//
//  emuone/NewVirtualMachineDialog.hpp - The modal "New VM" dialog
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
    namespace Ui { class NewVirtualMachineDialog; }

    /// \class NewVirtualMachineDialog emuone/API.hpp
    /// \brief The modal "New VM" dialog.
    class NewVirtualMachineDialog final
        :   public QDialog
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NewVirtualMachineDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< User's choice confirmed, VM created.
            Cancel  ///< User has cancelled the VM creation.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none
        explicit NewVirtualMachineDialog(
                QWidget * parent
            );

        /// \brief  The class destructor.
        virtual ~NewVirtualMachineDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Invoks the dialog modally.
        /// \return
        ///     The dialog result.
        Result          doModal();

        /// \brief
        ///     Returns the location of the newly created VM.
        /// \return
        ///     The full path to the configuration file newly
        ///     created VM, e"" if the dialog was cancelled.
        QString         virtualMachineLocation() const;

        //////////
        //  Implementation
    private:
        QString         _virtualMachineLocation;

        //  Helpers
        auto            _selectedArchitecture() -> emuone::core::IArchitecture *;
        void            _setSelectedArchitecture(emuone::core::IArchitecture * architecture);
        auto            _selectedType() -> emuone::core::IVirtualMachineType *;
        void            _setSelectedType(emuone::core::IVirtualMachineType * virtualMachineType);
        auto            _selectedTemplate() -> emuone::core::IVirtualMachineTemplate *;
        void            _setSelectedTemplate(emuone::core::IVirtualMachineTemplate * virtualMachineTemplate);
        void            _refresh();
        void            _refillTemplatesComboBox();

        //////////
        //  Controls
    private:
        Ui::NewVirtualMachineDialog *const  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _nameLineEditTextChanged(QString);
        void            _architectureComboBoxCurrentIndexChanged(int);
        void            _virtualMachineTypeComboBoxCurrentIndexChanged(int);
        void            _templateComboBoxCurrentIndexChanged(int);
        void            _locationLineEditTextChanged(QString);
        void            _browsePushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of emuone/NewVirtualMachineDialog.hpp

