//
//  emuone-scp360/DeviceDrivers.hpp
//
//  The SCP/360 device driver framework
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A generic "device driver"
    class EMUONE_SCP360_EXPORT DeviceDriver
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DeviceDriver)

        //////////
        //  Construction/destruction
    public:
        explicit DeviceDriver(ibm360::Device * device);
        virtual ~DeviceDriver();

        //////////
        //  Operations
    public:
        //  Creates a new DeviceDriver for the specified Device.
        //  Throws VirtualApplianceException if an error occurs (e.g. no such driver).
        static DeviceDriver *   create(ibm360::Device * device);

        //////////
        //  Implementation
    private:
        ibm360::Device *        _device;
    };

    //////////
    //  An IBM 2741 "device driver"
    class EMUONE_SCP360_EXPORT Ibm2741Driver : public DeviceDriver
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm2741Driver)

        //////////
        //  Construction/destruction
    public:
        explicit Ibm2741Driver(ibm360::Ibm2741 * ibm2741);
        virtual ~Ibm2741Driver();

        //////////
        //  Implementation
    private:
        ibm360::Ibm2741 *       _ibm2741;
    };
}

//  End of emuone-scp360/DeviceDrivers.hpp
