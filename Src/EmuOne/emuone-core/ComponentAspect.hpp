//
//  emuone-core/ComponentAspect.hpp
//
//  A "component aspect" framework
//
//////////
#pragma once

namespace core
{
    //////////
    //  A "component aspect" is a feature interface that a Component can implement
    //  if it is capable of a specificset of services
    class EMUONE_CORE_EXPORT IComponentAspect
    {
        //////////
        //  This is an interface
    public:
        virtual ~IComponentAspect() {}
    };

    //////////
    //  An aspect of a Component that is driven by a clock
    class EMUONE_CORE_EXPORT IClockedComponentAspect : public virtual IComponentAspect
    {
        //////////
        //  This is an interface
    public:
        virtual ~IClockedComponentAspect() {}

        //////////
        //  Operations
    public:
        //  The clock frequency of this component
        virtual ClockFrequency  clockFrequency() const = 0;

        //  Called on every clock tick when the component is "running"
        virtual void            onClockTick() = 0;
    };
}

//  End of emuone-core/ComponentAspect.hpp
