//
//  emuone-hades/kernel/Kernel.ProcessManagement.cpp - Process  management
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
//  Operations (process management)
StatusCode Kernel::createNativeProcess(
        Identity * owner,
        NativeExecutionEnvironment * executionEnvironment,
        Process * parent,
        PriorityClass priorityClass,
        const QString & name,
        const QString & command,
        const QString & commandLine,
        const QString & currentDirectory,
        PNativeProcess & nativeProcess
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    Q_ASSERT(executionEnvironment != nullptr &&
             executionEnvironment->kernel == this &&
             _objects.value(executionEnvironment->oid, nullptr) == executionEnvironment);
    Q_ASSERT((parent == nullptr) ||
             (parent->kernel == this &&
              _objects.value(parent->oid, nullptr) == parent));
    //  Create Native Process
    nativeProcess =
        new NativeProcess(
            this,
            generateUnusedOid(),
            owner,
            executionEnvironment,
            parent,
            priorityClass,
            name,
            command,
            commandLine,
            currentDirectory);
    Q_ASSERT(_objects.value(nativeProcess->oid, nullptr) == nativeProcess);
    Q_ASSERT(_processes.value(nativeProcess->oid, nullptr) == nativeProcess);
    return StatusCode::Success;
}

StatusCode Kernel::startProcess(Process * process)
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr && process->kernel == this);

    //  A process cannot be started more than once
    if (process->state != Process::State::Created)
    {   //  OOPS! Can't!
        return StatusCode::InvalidArgument;
    }
    //  Starting a Process with suspendCount > 0 just
    //  makes it Suspended...
    if (process->suspendCount > 0)
    {   //  ...and so is the case
        Q_ASSERT(false);    //  TODO implement
        return StatusCode::InvalidArgument;
    }
    //  Starting a Process means startiung all its Threads
    process->state = Process::State::Running;
    for (auto thread : process->threads)
    {
        startThread(thread);
    }
    //  We're done
    return StatusCode::Success;
}

//  End of emuone-hades/kernel/Kernel.ProcessManagement.cpp
