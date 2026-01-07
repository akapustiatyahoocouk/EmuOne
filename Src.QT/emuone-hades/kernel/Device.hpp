//
//  emuone-hades/kernel/Device.hpp - The HADES OS kernel Devices
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::hades::kernel
{
    /// \class Device emuone-hades/API.hpp
    /// \brief The device.
    class EMUONE_HADES_PUBLIC Device
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Device)

        friend class Kernel;
        friend class Processor;
        friend class ProcessorCore;
        friend class NativeExecutor;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        Device(
            Kernel * kernel, const Oid & oid, Identity * owner,
            DeviceType * deviceType,
            DeviceId deviceId, const QString & name);
        virtual ~Device();

        //////////
        //  Properties
        const DeviceId  deviceId;   //  within ght DeviceType
        const QString   name;

        //////////
        //  Associations
        const PDeviceType   deviceType;  //  counts as references
    };

    /// \class Processor emuone-hades/API.hpp
    /// \brief The device representing a Processor.
    class EMUONE_HADES_PUBLIC Processor
        :   public Device
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor)

        friend class Kernel;
        friend class ProcessorCore;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        Processor(
                Kernel * kernel, const Oid & oid, Identity * owner,
                DeviceType * deviceType,
                ProcessorId processorId
            );
        virtual ~Processor();

        //////////
        //  Properties
        const ProcessorId   processorId;    //  == deviceId, unique per Kernel

        //////////
        //  Associations
        ProcessorCores  cores;  //  count as "references"
    };

    /// \class ProcessorCore emuone-hades/API.hpp
    /// \brief The device representing a Processor Core.
    class EMUONE_HADES_PUBLIC ProcessorCore
        :   public Device
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ProcessorCore)

        friend class Kernel;
        friend class NativeExecutor;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        ProcessorCore(
                Kernel * kernel, const Oid & oid, Identity * owner,
                DeviceType * deviceType,
                Processor * processor,
                CoreId coreId, emuone::util::ByteOrder   byteOrder
            );
        virtual ~ProcessorCore();

        //////////
        //  Properties
        const CoreId    coreId;    //  lobyte of deviceId
        const emuone::util::ByteOrder   byteOrder;

        //////////
        //  Associations
        Processor *const    processor;  //  counts as "reference"
        NativeExecutor *    executor = nullptr; //  nullptr == core not used as an Executor; otherwise a "reference"
    };
}

//  End of emuone-hades/kernel/Device.hpp
