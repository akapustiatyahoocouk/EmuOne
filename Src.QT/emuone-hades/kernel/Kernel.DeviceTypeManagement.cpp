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
StatusCode Kernel::createDeviceType(
        Identity * owner,
        DeviceTypeId deviceTypeId,
        const QString & name,
        PDeviceType & deviceType
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceTypeId != DeviceTypeId::Invalid);

    //  Ensure deviceTypeId uniqueness
    if (_deviceTypes.contains(deviceTypeId))
    {   //  OOPS!
        deviceType = nullptr;
        return StatusCode::AlreadyExists;
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
    return StatusCode::Success;
}

QString Kernel::defaultDeviceTypeName(DeviceTypeId deviceTypeId)
{
    char s[64];

    switch (deviceTypeId)
    {
        case DeviceTypeId::Amd64Processor:
            return "AMD64";
        case DeviceTypeId::Amd64ProcessorCore:
            return "AMD64 core";
        default:
            sprintf(s, "Device type 0x%04X", deviceTypeId);
            return s;
    }
}

//  End of emuone-hades/kernel/Kernel.DeviceTypeManagement.cpp
