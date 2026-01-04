//
//  emuone-core/DisplaySurface.hpp - A full-window UI of a Running Component
//
//  EmuOne
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
#include "emuone-core/API.hpp"

    namespace emuone::core
{
    /// \class DisplaySurface emuone-core/API.hpp
    /// \brief A full-window UI of a Running Component.
    class EMUONE_CORE_PUBLIC DisplaySurface
        :   public QWidget
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DisplaySurface)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param component
        ///     The component to edit.
        DisplaySurface(QWidget * parent, IComponent * component);

        /// \brief
        ///     The class destructor.
        virtual ~DisplaySurface();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the component represented by this
        ///     DisplaySurface.
        /// \details
        ///     Derived classes should override this method for
        ///     return type covariance.
        /// \return
        ///     The component represented by this DisplaySurface.
        virtual auto    component() const -> IComponent * { return _component; }

        /// \brief
        ///     Returns the short user-readable name with which this
        ///     DisplaySurface appears in the VM's UI.
        /// \return
        ///     The short user-readable name with which this
        ///     DisplaySurface appears in the VM's UI.
        virtual QString displayName() const { return _component->displayName(); }

        //////////
        //  Implementation
    private:
        IComponent *const   _component; //  represented
    };
}

//  End of emuone-core/ComponentEditor.hpp
