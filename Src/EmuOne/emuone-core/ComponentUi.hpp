//
//  emuone-core/ComponentUi.hpp
//
//  An UI of a VM Component (when Running)
//
//////////
#pragma once

namespace core
{
    //////////
    //  An UI of a VM Component (when Running).
    //  A Component will create a ComponentUi when its "createUi()" method is called.
    //  The caller is responsible for "delete"-ing the ComponentUi instance when it is
    //  no longer needed.
    //  IMPORTANT: A ComponentUi must not out-live the Component for which it has been
    //             created!!! For safety, the Component should track all ComponentUi
    //             that it has created and, if "delete"-ed, "destroy"delete" them
    class EMUONE_CORE_EXPORT ComponentUi
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentUi)

        //////////
        //  Construction/destruction
    public:
        explicit ComponentUi(Component * component);
        virtual ~ComponentUi();

        //////////
        //  Implementation
    private:
        Component *const        _component;
    };
}

//  End of emuone-core/ComponentUi.hpp
