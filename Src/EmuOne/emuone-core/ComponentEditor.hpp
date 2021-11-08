//
//  emuone-core/ComponentEditor.hpp
//
//  The ComponentEditor ADT
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

namespace core
{
    //////////
    //  A generic "editor" of a Component's properties.
    //  A ComponentEditor must not out-live the Component whose properties it is editing.
    //  To that end, a Component must keep track of all the ComponentEditors it has created
    //  and "destroy "delete" them when the Component itself is "delet"ed.
    class EMUONE_CORE_EXPORT ComponentEditor : public QWidget
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentEditor)

        //////////
        //  Construction/destruction
    public:
        ComponentEditor(core::Component * component, QWidget * parent = nullptr);

        //////////
        //  Operations
    public:
        core::Component *   component() const { return _component; }

        //  Adjusts all the editor's fields to reflect the current configuration
        //  of the edited component.
        virtual void        refresh() = 0;

        //////////
        //  Signals
    signals:
        void                componentConfigurationChanged(core::Component * component);

        //////////
        //  Implementation
    private:
        core::Component *   _component;
    };
}

//  End of emuone-core/ComponentEditor.hpp

