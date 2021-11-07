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
        //  THe listener to completion of I/O requests that do not involve data transfer
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

        //  The listener to completion of I/O requests that involve data transfer
        class EMUONE_SCP360_EXPORT TransferCompletionListener
        {
            //////////
            //  This is an interface
        public:
            virtual ~TransferCompletionListener() {}

            //////////
            //  Operations
        public:
            virtual void        transferCompleted(Device * device, uint32_t bytesTransferred, ErrorCode errorCode) = 0;
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

        //  Initialises the specified "device" setting its State
        virtual ErrorCode       initialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) = 0;

        //  Deinitialises the specified "device"
        virtual ErrorCode       deinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) = 0;

        //  Writes a single record containing data from the specified "buffer"
        virtual ErrorCode       writeBlock(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) = 0;

        //  Writes a stream of bytes containing data from the specified "buffer"
        virtual ErrorCode       write(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) = 0;

        //  Reads a single record containing data into the specified "buffer"
        virtual ErrorCode       readBlock(Device * device, util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) = 0;

        //  Reads a stream of bytes containing data into the specified "buffer"
        virtual ErrorCode       read(Device * device, util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) = 0;

        //  Terminates any ongoing I/O operation on the specified "device" ASAP.
        virtual ErrorCode       haltIo(Device * device) = 0;

        virtual ErrorCode       validateOpen(OpenFileFlags openFlags, uint32_t recordSize, uint32_t blockSize) const = 0;

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
        virtual ErrorCode       initialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) override;
        virtual ErrorCode       deinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener) override;
        virtual ErrorCode       writeBlock(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) override;
        virtual ErrorCode       write(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) override;
        virtual ErrorCode       readBlock(Device * device, util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) override;
        virtual ErrorCode       read(Device * device, util::Buffer * buffer, TransferCompletionListener * transferCompletionListener) override;
        virtual ErrorCode       haltIo(Device * device) override;
        virtual ErrorCode       validateOpen(OpenFileFlags openFlags, uint32_t recordSize, uint32_t blockSize) const override;

        //////////
        //  Implementation
    private:
        QSet<ibm360::Ibm2741*>  _knownHardwareDevices;

        class _Ibm2741IoCompletionListener : public ibm360::Ibm2741::IoCompletionListener
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Ibm2741IoCompletionListener)

            //////////
            //  Construction/destruction
        public:
            _Ibm2741IoCompletionListener(Ibm2741Driver * driver, Device * device)
                :   _driver(driver), _device(device) {}
            virtual ~_Ibm2741IoCompletionListener() {}

            //////////
            //  ibm360::Ibm2741::IoCompletionListener
        public:
            virtual void        ioCompleted(ibm360::Ibm2741::ErrorCode errorCode) override;

            //////////
            //  Properties
        public:
            Ibm2741Driver::IoCompletionListener * ioCompletionListener = nullptr;

            //////////
            //  Implementation
        public:
            Ibm2741Driver *const    _driver;
            Device *const           _device;
        };
        QSet<_Ibm2741IoCompletionListener*> _ibm2741IoCompletionListeners;

        class _Ibm2741TransferCompletionListener : public ibm360::Ibm2741::TransferCompletionListener
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Ibm2741TransferCompletionListener)

            //////////
            //  Construction/destruction
        public:
            _Ibm2741TransferCompletionListener(Ibm2741Driver * driver, Device * device)
                :   _driver(driver), _device(device) {}
            virtual ~_Ibm2741TransferCompletionListener() {}

            //////////
            //  ibm360::Ibm2741::TransferCompletionListener
        public:
            virtual void        transferCompleted(uint32_t bytesTransferred, ibm360::Ibm2741::ErrorCode errorCode) override;

            //////////
            //  Properties
        public:
            Ibm2741Driver::TransferCompletionListener * transferCompletionListener = nullptr;

            //////////
            //  Implementation
        public:
            Ibm2741Driver *const    _driver;
            Device *const           _device;
        };
        QSet<_Ibm2741TransferCompletionListener*>   _ibm2741TransferCompletionListeners;

        //  Helpers
        static ibm360::Ibm2741 *_resolveIbm2741(Device * device);
        static ErrorCode        _translateErrorCode(ibm360::Ibm2741::ErrorCode ibm2741ErrorCode);
    };
}

//  End of emuone-scp360/DeviceDrivers.hpp
