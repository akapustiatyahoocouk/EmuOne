//
//  emuone-core/ComponentAdaptorEditor.hpp -ComponentAdaptor property editor widget
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
    /// \class ComponentAdaptorEditor emuone-core/API.hpp
    /// \brief A generic component adaptor editor.
    class EMUONE_CORE_PUBLIC ComponentAdaptorEditor
        :   public QWidget
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentAdaptorEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the editor.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param componentAdaptor
        ///     The component adaptor to edit.
        ComponentAdaptorEditor(
                QWidget * parent,
                IComponentAdaptor * componentAdaptor
            );

        /// \brief
        ///     The class destructor.
        virtual ~ComponentAdaptorEditor();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the component adaptor being edited.
        /// \details
        ///     Derived classes should override this method for
        ///     return type covariance.
        /// \return
        ///     The component adaptor being edited.
        virtual auto    componentAdaptor() const -> IComponentAdaptor * { return _componentAdaptor; }

        //////////
        //  Implementation
    private:
        IComponentAdaptor *const    _componentAdaptor; //  being edited
    };
}

//  End of emuone-core/ComponentAdaptorEditor.hpp
