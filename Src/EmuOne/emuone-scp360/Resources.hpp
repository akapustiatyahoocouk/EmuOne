//
//  emuone-scp360/Resources.hpp
//
//  The SCP/360 Resources
//
//////////
#pragma once

namespace scp360
{

    //////////
    //  A generic SCP/360 "Resource" is an entity to which one or several Processes
    //  can have open "handles"
    class EMUONE_SCP360_EXPORT IResource
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IResource)

        //////////
        //  Construction/destruction
    public:
        explicit IResource(Scp * scp) : _scp(scp) { Q_ASSERT(_scp != nullptr); }
        virtual ~IResource() {}

        //////////
        //  Operations
    public:
        //  The total number of "handles" to this Resource that are open by all Processes
        uint32_t            openHandleCount() const { return _openHandleCount; }

        //  Increment the "open handles" count; retrns true on success, false on failure (overflow)
        bool                incrementOpenHandleCount();

        //  Decrement the "open handles" count
        void                decrementOpenHandleCount();

        //////////
        //  Implementation
    private:
        Scp *const          _scp;   //  ...managing this Resource
        uint32_t            _openHandleCount = 0;
    };

    //////////
    //  A "resource" that can be used for I/O purposes.
    class EMUONE_SCP360_EXPORT IIoResource : public virtual IResource
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IIoResource)

        //////////
        //  Construction/destruction
    public:
        IIoResource(Scp * scp, OpenFileFlags openFileFlags) : IResource(scp), _openFileFlags(openFileFlags) {}
        virtual ~IIoResource() {}

        //////////
        //  Operations
    public:
        OpenFileFlags       openFileFlags() const { return _openFileFlags; }

        //////////
        //  Implementation
    private:
        OpenFileFlags       _openFileFlags;
    };

    //////////
    //  A "resource" that represents a Device for I/O purposes.
    //  "DeviceResource"s are created when a Process issues an "open file" system call
    //  where "file name" refers to a physical (TODO or logical) device
    class EMUONE_SCP360_EXPORT DeviceResource : public virtual IIoResource
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DeviceResource)

        //////////
        //  Construction/destruction
    public:
        DeviceResource(Scp * scp, OpenFileFlags openFileFlags, Device * device)
            :   IResource(scp),
                IIoResource(scp, openFileFlags),
                _device(device)
        {
            Q_ASSERT(_device != nullptr);
        }
        virtual ~DeviceResource() {}

        //////////
        //  Operations
    public:
        Device *            device() const { return _device; }

        //////////
        //  Implementation
    private:
        Device *            _device;
    };
}

//  End of emuone-scp360/Resources.hpp
