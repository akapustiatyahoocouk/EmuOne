//
//  emuone/StoppedVirtualMachineView.hpp - The Stopped VM view
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
    namespace Ui { class StoppedVirtualMachineView; }

    /// \class StoppedVirtualMachineView emuone/API.hpp
    /// \brief The view of a Stopped VM.
    class StoppedVirtualMachineView final
        :   public QWidget
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StoppedVirtualMachineView)

        //////////
        //  Construction/destruction
    public:
        StoppedVirtualMachineView(
                QWidget * parent,
                emuone::core::VirtualMachine * virtualMachine
            );
        virtual ~StoppedVirtualMachineView();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Refreshes this view.
        void            refresh();

        //////////
        //  Implementation
    private:
        emuone::core::VirtualMachine *const _virtualMachine;

        //////////
        //  Controls
    private:
        Ui::StoppedVirtualMachineView *const    _ui;
    };
}

//  End of emuone/StoppedVirtualMachineView.hpp

