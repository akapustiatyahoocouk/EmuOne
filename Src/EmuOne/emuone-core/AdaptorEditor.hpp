//
//  emuone-core/AdaptorEditor.hpp
//
//  The AdaptorEditor ADT
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

namespace core
{
    //////////
    //  A generic "editor" of a Adaptor's properties.
    //  A AdaptorEditor must not out-live the Adaptor whose properties it is editing.
    //  To that end, a Adaptor must keep track of all the AdaptorEditors it has created
    //  and "destroy "delete" them when the Adaptor itself is "delet"ed.
    class EMUONE_CORE_EXPORT AdaptorEditor : public QWidget
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AdaptorEditor)

        //////////
        //  Construction/destruction
    public:
        AdaptorEditor(core::Adaptor * adaptor, QWidget * parent = nullptr);

        //////////
        //  Operations
    public:
        core::Adaptor *     adaptor() const { return _adaptor; }

        //  Adjusts all the editor's fields to reflect the current configuration
        //  of the edited adaptor.
        virtual void        refresh() = 0;

        //////////
        //  Signals
    signals:
        void                adaptorConfigurationChanged(core::Adaptor * adaptor);

        //////////
        //  Implementation
    private:
        core::Adaptor *     _adaptor;
    };
}

//  End of emuone-core/AdaptorEditor.hpp

