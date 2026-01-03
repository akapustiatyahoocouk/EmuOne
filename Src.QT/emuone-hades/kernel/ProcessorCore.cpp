//
//  emuone-hades/kernel/ProcessorCore.cpp - emuone::hades::kernel::ProcessorCore class implementation
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
ProcessorCore::ProcessorCore(
        Kernel * kernel, const Oid & oid, Identity * owner,
        DeviceType * deviceType,
        Processor * processor,
        CoreId coreId
    ) : Device(
            kernel, oid, owner,
            deviceType,
            DeviceId((int(processor->processorId) << 8) | int(coreId)),
            processor->name + "c" + emuone::util::toString(int(coreId))),
        //  Properties
        coreId(coreId),
        //  Associations
        processor(processor)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(deviceType != nullptr &&
             deviceType->kernel == kernel &&
             kernel->_deviceTypes.value(deviceType->deviceTypeId, nullptr) == deviceType);
    Q_ASSERT(processor != nullptr &&
             processor->kernel == kernel &&
             kernel->_processors.value(processor->processorId, nullptr) == processor);
    Q_ASSERT(deviceType->deviceTypeId == DeviceTypeId::HostProcessorCore);

    //  Link ProcessorCore with Processor
    processor->cores.insert(this);
    this->referenceCount++;
    deviceType->referenceCount++;
}

ProcessorCore::~ProcessorCore()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  Un-link ProcessorCore from Processor
        Q_ASSERT(processor->cores.contains(this));
        processor->cores.remove(this);
        Q_ASSERT(processor->referenceCount > 0);
        processor->referenceCount--;
        Q_ASSERT(this->referenceCount > 0);
        this->referenceCount--;
    }
}

//  End of emuone-hades/kernel/ProcessorCore.cpp
