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
        //  Construxction/destruction
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
        //  Controls
    private:
        Ui::NewVirtualMachineDialog *const  _ui;
    };
}

//  End of emuone/NewVirtualMachineDialog.hpp

