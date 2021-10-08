//
//  emuone-ibm360/Device.hpp
//
//  A generic "device"
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  A "device" is something that has a 11-bit I/O address and is accessed via Channels
    class EMUONE_IBM360_EXPORT Device : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Device)

        //////////
        //  Construction/destruction
    public:
        Device(const QString & name, uint16_t address);
        virtual ~Device();

        //////////
        //  Component (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        static bool             isValidAddress(uint16_t address);
        uint16_t                address() const { return _address; }
        void                    setAddress(uint16_t address);

        //////////
        //  Implementation
    private:
        uint16_t                _address;
    };
}

//  End of emuone-ibm360/Device.hpp
