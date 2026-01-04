//
//  emuone-hades/kernel/DeviceType.cpp - emuone::hades::kernel::DeviceType class implementation
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
DeviceType::DeviceType(
        Kernel * kernel, const Oid & oid, Identity * owner,
        DeviceTypeId deviceTypeId, const QString & name
    ) : Object(kernel, oid, owner),
        //  Properties
        deviceTypeId(deviceTypeId),
        name(name)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceTypeId.isValid());

    //  Add to Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(!kernel->_deviceTypes.contains(deviceTypeId));
    kernel->_deviceTypes[deviceTypeId] = this;
}

DeviceType::~DeviceType()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Remove from Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_deviceTypes.value(deviceTypeId, nullptr) == this);
    kernel->_deviceTypes.remove(deviceTypeId);
}

//  End of emuone-hades/kernel/DeviceType.cpp
