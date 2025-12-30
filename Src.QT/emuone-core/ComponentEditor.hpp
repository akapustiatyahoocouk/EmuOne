//
//  emuone-core/ComponentEditor.hpp -Component property editor widget
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
    /// \class ComponentEditor emuone-core/API.hpp
    /// \brief A generic component editor.
    class EMUONE_CORE_PUBLIC ComponentEditor
        :   public QWidget
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the editor.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param component
        ///     The component to edit.
        ComponentEditor(QWidget * parent, IComponent * component);

        /// \brief
        ///     The class destructor.
        virtual ~ComponentEditor();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the component being edited.
        /// \details
        ///     Derived classes should override this method for
        ///     return type covariance.
        /// \return
        ///     The component being edited.
        virtual auto    component() const -> IComponent * { return _component; }

        /// \brief
        ///     Checks whether controls in this editor have valid values.
        /// \return
        ///     True if controls in this editor have valid values, else false.
        virtual bool    isValid() const = 0;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when one of the editor's controls changes
        ///     their value.
        void            valueChanged();

        //////////
        //  Implementation
    private:
        IComponent *const   _component; //  being edited
    };
}

//  End of emuone-core/ComponentEditor.hpp
