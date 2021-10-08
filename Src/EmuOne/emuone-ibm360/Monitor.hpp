//
//  emuone-ibm360/Monitor.hpp
//
//  IBM/360 Monitor
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  An IBM/360 "Monitor" is a software componen that takes the role of
    //  an OS running on a IBM/360 machine
    class EMUONE_IBM360_EXPORT Monitor : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Monitor)

        //////////
        //  Construction/destruction
    public:
        explicit Monitor(const QString & name) : core::Component(name) {}
        virtual ~Monitor() {}
    };
}

//  End of emuone-ibm360/Monitor.hpp
