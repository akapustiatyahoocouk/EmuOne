//
//  emuone-hades/kernel/Processor.cpp - emuone::hades::kernel::Processor class implementation
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
Processor::Processor(
        Kernel * kernel,
        const Oid & oid,
        Identity * owner,
        DeviceType * deviceType,
        ProcessorId processorId
    ) : Device(
            kernel, oid, owner,
            deviceType,
            DeviceId(processorId),
            "cpu" + emuone::util::toString(int(processorId))),
        //  Properties
        processorId(processorId)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceType->deviceTypeId == DeviceTypeId::HostProcessor);

    //  Add to Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(!kernel->_processors.contains(processorId));
    kernel->_processors[processorId] = this;
}

Processor::~Processor()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Remove from Kernel's secondarycaches
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_processors.value(processorId, nullptr) == this);
    kernel->_processors.remove(processorId);
}

//  End of emuone-hades/kernel/Processor.cpp
