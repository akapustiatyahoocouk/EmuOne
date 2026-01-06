//
//  emuone-hades/kernel/Kernel.ThreadManagement.cpp - Thread  management
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
KErrno Kernel::createNativeThread(
        Identity * owner,
        NativeProcess * process,
        PriorityClass priorityClass,
        const QString name,
        NativeThreadRunner * runner,
        PNativeThread & nativeThread
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(owner != nullptr &&
             owner->kernel == this &&
             _objects.value(owner->oid, nullptr) == owner);
    Q_ASSERT(process != nullptr &&
             process->kernel == this &&
             _objects.value(process->oid, nullptr) == process);
    Q_ASSERT(runner != nullptr && runner->_nativeThread == nullptr);

    //  Create Native Thread
    nativeThread =
        new NativeThread(
            this,
            generateUnusedOid(),
            owner,
            process,
            priorityClass,
            name,
            runner);
    Q_ASSERT(_objects.value(nativeThread->oid, nullptr) == nativeThread);
    return K_EOK;
}

KErrno Kernel::startThread(Thread * thread)
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(thread != nullptr && thread->kernel == this);

    //  A Thread must belong to a Running (TODO or Suspended) process
    if (thread->process->state != Process::State::Running)  //  TODO or suspended
    {
        return K_ENOTSUP;
    }
    //  A thread cannot be started more than once
    if (thread->state != Thread::State::Created)
    {   //  OOPS! Can't!
        return K_ENOTSUP;
    }
    //  Starting a Process with suspendCount > 0 just
    //  makes it Suspended...
    if (thread->suspendCount > 0 ||
        thread->process->suspendCount > 0)
    {   //  ...and so is the case
        Q_ASSERT(false);    //  TODO implement
        return K_ENOTSUP;
    }
    //  We can now start the Thread
    if (auto nativeThread =
        dynamic_cast<NativeThread*>(thread))
    {
        thread->state = Thread::State::Running;
        Q_ASSERT(nativeThread->_runnerThread == nullptr);
        nativeThread->_runnerThread =
            new NativeThread::_RunnerThread(nativeThread);
        nativeThread->_runnerThread->start();   //  TODO priority
    }
    else
    {   //  Emulated thread
        Q_ASSERT(false);    //  TODO implement
        thread->state = Thread::State::Waiting;
    }
    //  All done
    return K_EOK;
}

KErrno Kernel::terminateThread(Thread * thread, uint32_t exitCode)
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(thread != nullptr && thread->kernel == this);

    if (auto nativeThread =
        dynamic_cast<NativeThread*>(thread))
    {   //  If there is a QThread, kill it
        if (nativeThread->_runnerThread != nullptr)
        {
            nativeThread->_runnerThread->terminate();
            delete nativeThread->_runnerThread;
            nativeThread->_runnerThread = nullptr;
        }
    }

    //  Lose the Thread's "affinity"
    for (auto e : Executors(thread->affinity))   //  Shallow clone
    {
        Q_ASSERT(e->affineThreads.contains(thread));
        Q_ASSERT(e->referenceCount > 0);
        Q_ASSERT(thread->referenceCount > 0);
        e->affineThreads.remove(thread);
        e->referenceCount--;
        thread->referenceCount--;
    }
    thread->affinity.clear();

    //  TODO other cleanup
    //  TODO what can we join with the cleanup code
    //  at the end of NativeThread::_RunnerThread::run() ?

    //  The Thread is now Finished
    thread->state = Thread::State::Finished;
    thread->exitCode = exitCode;
    return K_EOK;
}

//  End of emuone-hades/kernel/Kernel.ThreadManagement.cpp

