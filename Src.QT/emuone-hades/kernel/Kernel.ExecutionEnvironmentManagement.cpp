//
//  emuone-hades/kernel/Kernel.ExecutionEnvironmentManagement.cpp - Execution Environment  management
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
//  Operations (execution environment management)
KErrno Kernel::createNativeExecutionEnvironment(
        Identity * owner,
        const QString & name,
        const NativeExecutors & nativeExecutors,
        PNativeExecutionEnvironment & nativeExecutionEnvironment
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    for (auto nativeExecutor : nativeExecutors)
    {
        Q_ASSERT(nativeExecutor != nullptr &&
                 nativeExecutor->kernel == this &&
                 _objects.value(nativeExecutor->oid, nullptr) == nativeExecutor &&
                 nativeExecutor->byteOrder() == emuone::util::ByteOrder::Host);
    }

    //  Ensure EE name uniqueness
    if (_executionEnvironments.contains(name))
    {   //  OOPS!
        nativeExecutionEnvironment = nullptr;
        return K_EEXIST;
    }

    //  Create new Execution Environment
    nativeExecutionEnvironment =
        new NativeExecutionEnvironment(
            this,
            generateUnusedOid(),
            owner,
            name,
            nativeExecutors);
    Q_ASSERT(_executionEnvironments.value(nativeExecutionEnvironment->name) == nativeExecutionEnvironment);
    Q_ASSERT(_objects.value(nativeExecutionEnvironment->oid, nullptr) == nativeExecutionEnvironment);
    return K_EOK;
}

//  End of emuone-hades/kernel/Kernel.ExecutionEnvironmentManagement.cpp
