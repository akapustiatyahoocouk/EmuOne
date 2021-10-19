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
    class EMUONE_SCP360_EXPORT Resource
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Resource)

        //////////
        //  Construction/destruction
    public:
        Resource() {}
        virtual ~Resource() {}

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
        uint32_t            _openHandleCount = 0;
    };

    //////////
    //  A "resource" that represents a Device for I/O purposes.
    //  "DeviceResource"s are created when a Process issues an "open file" system call
    //  where "file name" refers to a physical (TODO or logical) device
    class EMUONE_SCP360_EXPORT DeviceResource : public Resource
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DeviceResource)

        //////////
        //  Construction/destruction
    public:
        DeviceResource(Device * device) : _device(device) { Q_ASSERT(_device != nullptr); }
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
