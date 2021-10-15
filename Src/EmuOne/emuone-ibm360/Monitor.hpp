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

        //////////
        //  Operations - interrupt handling.
        //  All of them return when SCP is ready to switch back to "user mode".
        //  Al of them are "queued" on SCP, to be acted upon as soon as practicable.
    public:
        virtual void        onIoInterruption(uint16_t interruptionCode) = 0;
        virtual void        onProgramInterruption(uint16_t interruptionCode) = 0;
        virtual void        onSvcInterruption(uint16_t interruptionCode) = 0;
        virtual void        onExternalInterruption(uint16_t interruptionCode) = 0;
        virtual void        onMachineCheckInterruption(uint16_t interruptionCode) = 0;
    };
}

//  End of emuone-ibm360/Monitor.hpp
