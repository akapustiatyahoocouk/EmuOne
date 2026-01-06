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
KErrno Kernel::createNativeProcess(
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
    return K_EOK;
}

KErrno Kernel::startProcess(Process * process)
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr && process->kernel == this);

    //  A process cannot be started more than once
    if (process->state != Process::State::Created)
    {   //  OOPS! Can't!
        return K_ENOTSUP;
    }
    //  Starting a Process with suspendCount > 0 just
    //  makes it Suspended...
    if (process->suspendCount > 0)
    {   //  ...and so is the case
        Q_ASSERT(false);    //  TODO implement
        return K_ENOTSUP;
    }
    //  Starting a Process means startiung all its Threads
    process->state = Process::State::Running;
    for (auto thread : std::as_const(process->threads))
    {
        startThread(thread);
    }
    //  We're done
    return K_EOK;
}

bool Kernel::isReadyToDie(Process * process)
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr && process->kernel == this);

    //  A Process is ready to die when it has no Running or
    //  Suspended non-daemon Threads
    for (auto t : std::as_const(process->threads))
    {
        Q_ASSERT(!t->isDaemon); //  TODO implement
        if (t->state != Thread::State::Created &&
            t->state != Thread::State::Finished)
        {
            return false;
        }
    }
    return true;
}

//  End of emuone-hades/kernel/Kernel.ProcessManagement.cpp
