//
//  emuone-hades/kernel/Device.cpp - emuone::hades::kernel::Device class implementation
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
//  Construction/destruction
Device::Device(
        Kernel * kernel, const Oid & oid, Identity * owner,
        DeviceType * deviceType,
        DeviceId deviceId, const QString & name
    ) : Object(kernel, oid, owner),
        //  Properties
        deviceId(deviceId),
        name(name),
        //  Associations
        deviceType(deviceType)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceType != nullptr &&
             deviceType->kernel == kernel &&
             kernel->_deviceTypes.value(deviceType->deviceTypeId, nullptr) == deviceType);

    //  Link Device with DeviceType
    deviceType->devices.insert(this);
    this->referenceCount++;
    deviceType->referenceCount++;
}

Device::~Device()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  Un-link Device from DeviceType
        Q_ASSERT(deviceType->devices.contains(this));
        deviceType->devices.remove(this);
        Q_ASSERT(deviceType->referenceCount > 0);
        deviceType->referenceCount--;
        Q_ASSERT(this->referenceCount > 0);
        this->referenceCount--;
    }
}

//  End of emuone-hades/kernel/Device.cpp
