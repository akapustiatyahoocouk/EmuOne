//
//  emuone-hades/kernel/Kernel.DeviceManagement.cpp - Device  management
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
//  Operations (device management)
StatusCode Kernel::createProcessor(
        Identity * owner,
        DeviceType * deviceType,
        ProcessorId processorId,
        PProcessor & processor
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    Q_ASSERT(deviceType != nullptr &&
             deviceType->kernel == this &&
             _objects.value(deviceType->oid, nullptr) == deviceType &&
             deviceType->deviceTypeId == DeviceTypeId::HostProcessor);
    Q_ASSERT(processorId.isValid());

    //  Ensure uniqueness
    if (_processors.contains(processorId))
    {   //  OOPS!
        processor = nullptr;
        return StatusCode::AlreadyExists;
    }

    //  Create Processor
    processor =
        new Processor(
            this,
            generateUnusedOid(),
            owner,
            deviceType,
            processorId);
    Q_ASSERT(_processors.value(processorId) == processor);
    Q_ASSERT(_objects.value(processor->oid, nullptr) == processor);
    return StatusCode::Success;
}

StatusCode Kernel::createProcessorCore(
        Identity * owner,
        DeviceType * deviceType,
        Processor * processor,
        CoreId coreId,
        PProcessorCore & processorCore
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    Q_ASSERT(deviceType != nullptr &&
             deviceType->kernel == this &&
             _objects.value(deviceType->oid, nullptr) == deviceType &&
             deviceType->deviceTypeId == DeviceTypeId::HostProcessorCore);
    Q_ASSERT(processor != nullptr &&
             processor->kernel == this &&
             _objects.value(processor->oid, nullptr) == processor);
    Q_ASSERT(coreId.isValid());

    //  TODO uniqueness of processor ID within the Processor

    //  Create ProcessorCore
    processorCore =
        new ProcessorCore(
            this,
            generateUnusedOid(),
            owner,
            deviceType,
            processor,
            coreId,
            emuone::util::ByteOrder::Host);
    //  TODO Q_ASSERT(_processorCores.value(coreId) == processorCore);
    Q_ASSERT(_objects.value(processor->oid, nullptr) == processor);
    return StatusCode::Success;
}

//  End of emuone-hades/kernel/Kernel.DeviceManagement.cpp
