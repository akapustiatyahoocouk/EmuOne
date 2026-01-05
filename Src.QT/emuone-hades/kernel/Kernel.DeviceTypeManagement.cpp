//
//  emuone-hades/kernel/Kernel.DeviceTypeManagement.cpp - Device type management
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
#include "emuone-hades/API.hpp"
using namespace emuone::hades::kernel;

//////////
//  Operations (device type management)
KErrno Kernel::createDeviceType(
        Identity * owner,
        DeviceTypeId deviceTypeId,
        const QString & name,
        PDeviceType & deviceType
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceTypeId.isValid());

    //  Ensure deviceTypeId uniqueness
    if (_deviceTypes.contains(deviceTypeId))
    {   //  OOPS!
        deviceType = nullptr;
        return K_EEXIST;
    }

    //  Create new DeviceType.
    deviceType =
        new DeviceType(
            this,
            generateUnusedOid(),
            owner,
            deviceTypeId,
            name);
    Q_ASSERT(_deviceTypes.value(deviceTypeId) == deviceType);
    Q_ASSERT(_objects.value(deviceType->oid, nullptr) == deviceType);
    return K_EOK;
}

QString Kernel::defaultDeviceTypeName(DeviceTypeId deviceTypeId)
{
    if (deviceTypeId == DeviceTypeId::Amd64Processor)
    {
        return "AMD64";
    }
    if (deviceTypeId == DeviceTypeId::Amd64ProcessorCore)
    {
        return "AMD64 core";
    }
    //  Give up
    char s[64];
    sprintf(s, "Device type 0x%04X", static_cast<uint16_t>(deviceTypeId));
    return s;
}

//  End of emuone-hades/kernel/Kernel.DeviceTypeManagement.cpp
