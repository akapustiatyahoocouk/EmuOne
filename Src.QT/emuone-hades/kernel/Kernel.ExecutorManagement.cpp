//
//  emuone-hades/kernel/Kernel.ExecutorManagement.cpp - Executor  management
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
//  Operations (executor management)
KErrno Kernel::createNativeExecutor(
        Identity * owner,
        ProcessorCore * processorCore,
        PNativeExecutor & nativeExecutor
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    Q_ASSERT(processorCore != nullptr &&
             processorCore->kernel == this &&
             _objects.value(processorCore->oid, nullptr) == processorCore &&
             processorCore->executor == nullptr);

    //  Ensure Executor name uniqueness
    if (_executors.contains(processorCore->name))
    {   //  OOPS!
        nativeExecutor = nullptr;
        return K_EEXIST;
    }

    //  Create new Executor
    nativeExecutor =
        new NativeExecutor(
            this,
            generateUnusedOid(),
            owner,
            processorCore);
    Q_ASSERT(_executors.value(nativeExecutor->name) == nativeExecutor);
    Q_ASSERT(_objects.value(nativeExecutor->oid, nullptr) == nativeExecutor);
    return K_EOK;
}

//  End of emuone-hades/kernel/Kernel.ExecutorManagement.cpp
