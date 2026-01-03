//
//  emuone-hades/kernel/DeviceType.hpp - The HADES OS kernel Device types
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
    /// \class DeviceType emuone-hades/API.hpp
    /// \brief The device type.
    class EMUONE_HADES_PUBLIC DeviceType final
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DeviceType)

        friend class Kernel;
        friend class Device;
        friend class Processor;
        friend class ProcessorCore;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        DeviceType(Kernel * kernel, const Oid & oid, Identity * owner,
                   DeviceTypeId deviceTypeId, const QString & name);
        virtual ~DeviceType();

        //////////
        //  Properties
        const DeviceTypeId  deviceTypeId;
        const QString       name;

        //////////
        //  Associations
        Devices             devices;    //  count as references
    };
}

//  End of emuone-hades/kernel/DeviceType.hpp
