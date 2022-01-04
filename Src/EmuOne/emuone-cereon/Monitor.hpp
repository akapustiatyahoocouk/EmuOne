//
//  emuone-cereon/Monitor.hpp
//
//  Cereon Monitor
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  A Cereon "Monitor" is a software componen that takes the role of
    //  an OS running on a Cereon machine
    class EMUONE_CEREON_EXPORT Monitor : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Monitor)

        //////////
        //  Construction/destruction
    public:
        explicit Monitor(const QString & name) : core::Component(name) {}
        virtual ~Monitor() {}

        //////////
        //  Operations - interrupt handling.
    public:
    };
}

//  End of emuone-cereon/Monitor.hpp
