//
//  emuone/RunningVirtualMachineView.hpp - The Running VM view
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
    namespace Ui { class RunningVirtualMachineView; }

    /// \class RunningVirtualMachineView emuone/API.hpp
    /// \brief The view of a Running VM.
    class RunningVirtualMachineView final
        :   public QWidget
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RunningVirtualMachineView)

        //////////
        //  Construction/destruction
    public:
        RunningVirtualMachineView(
                QWidget * parent,
                emuone::core::VirtualMachine * virtualMachine
            );
        virtual ~RunningVirtualMachineView();

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

        //  UI elements - created ONCE for a Running VM
        bool            _uiCreated = false;
        QMap<emuone::core::IComponent*,
             emuone::core::IComponent::IUi*>    _componentUis;  //  native AND adapted
        QMap<emuone::core::IComponentAdaptor*,
             emuone::core::IComponentAdaptor::IUi*> _adaptorUis;

        QList<emuone::core::DisplaySurface*>    _displaySurfaces;   //  In tab order

        //  Helpers
        void            _createUi();
        void            _destroyUi();

        //////////
        //  Controls
    private:
        Ui::RunningVirtualMachineView *const    _ui;
    };
}

//  End of emuone/RunningVirtualMachineView.hpp

