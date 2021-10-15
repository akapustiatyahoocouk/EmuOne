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
        //  Types
    public:
        class EMUONE_SCP360_EXPORT IoCompletionListener
        {
            //////////
            //  This is an interface
        public:
            virtual ~IoCompletionListener() {}

            //////////
            //  Operations
        public:
            virtual void        ioCompleted(Device * device, ErrorCode errorCode) = 0;
        };

        //////////
        //  Construction/destruction
    public:
        DeviceDriver();
        virtual ~DeviceDriver();

        //////////
        //  Operations
    public:
        static DeviceDriver *   createHardwareDeviceDriver(ibm360::Device * hardwareDevice);    //  throws VirtualApplianceException on error

        //  Retrns the Flags indicating capabilities of the driven Devices
        virtual Device::Flags   deviceFlags() const = 0;

        //  Initiates an I/O operation on the specified Device.
        //  If the I/O operation could not be started, returns immediately with a
        //  non-OK error code. Otherwise returns OK error code AND guarantees that the
        //  specified "ioCompletionListener" will be notified (on an arbitrary worker
        //  thread internal to the DeviceDriver or Device) when I/O operation has finished
        virtual ErrorCode       beginInitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) = 0;
        virtual ErrorCode       beginDeinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) = 0;

        //////////
        //  Implementation
    private:
    };

    //////////
    //  An IBM 2741 "device driver"
    class EMUONE_SCP360_EXPORT Ibm2741Driver : public DeviceDriver
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm2741Driver)

        //////////
        //  Construction/destruction
    public:
        Ibm2741Driver();
        virtual ~Ibm2741Driver();

        //////////
        //  DeviceDriver
    public:
        virtual Device::Flags   deviceFlags() const override;
        virtual ErrorCode       beginInitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) override;
        virtual ErrorCode       beginDeinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) override;

        //////////
        //  Implementation
    private:
    };
}

//  End of emuone-scp360/DeviceDrivers.hpp
