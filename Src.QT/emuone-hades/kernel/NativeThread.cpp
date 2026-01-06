//
//  emuone-hades/kernel/NativeThread.cpp - emuone::hades::kernel::NativeThread class implementation
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
NativeThread::NativeThread(
        Kernel * kernel, const Oid & oid, Identity * owner,
        NativeProcess * process,
        PriorityClass priorityClass,
        const QString name,
        NativeThreadRunner * runner
    ) : Thread(
            kernel, oid, owner,
            process,
            priorityClass,
            name,
            process->executionEnvironment->affinity),
        _runner(runner)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(_runner != nullptr && _runner->_nativeThread == nullptr);

    //  Take over the Runner
    _runner->_nativeThread = this;

    //  Add to secondary Kernel caches
    kernel->_nativeThreads.insert(this);
}

NativeThread::~NativeThread()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    Q_ASSERT(_runner != nullptr);
    delete _runner;

    Q_ASSERT(_runnerThread == nullptr ||
             !_runnerThread->isRunning());

    // Remove from secondary Kernel caches
    Q_ASSERT(kernel->_nativeThreads.contains(this));
    kernel->_nativeThreads.remove(this);
}

//////////
//  NativeThread::_RunnerThread
NativeThread::_RunnerThread::_RunnerThread(NativeThread * nativeThread)
    :   _nativeThread(nativeThread)
{
    Q_ASSERT(_nativeThread != nullptr);
}

void NativeThread::_RunnerThread::run()
{
    Q_ASSERT(_nativeThread != nullptr &&
             _nativeThread->_runner != nullptr);

    uint32_t exitCode;
    try
    {
        exitCode = _nativeThread->_runner->run();
    }
    catch (const emuone::util::Exception & ex)
    {   //  OIOPS! Log & exit thread
        qCritical() << ex;
        exitCode = 0xFFFFFFFF;
    }
    catch (const emuone::util::Error & ex)
    {   //  OOPS! Log & exit thread
        qCritical() << ex;
        exitCode = 0xFFFFFFFF;
    }
    catch (uint32_t ec)
    {   //  Exit thread
        exitCode = ec;
    }
    //  TODO what about "exit process" ?
    catch (...)
    {   //  OOPS! Can't log, but must still exit thread
        exitCode = 0xFFFFFFFF;
    }
    //  The NativeThread has now Finished
    //  We need to make state change in Kernel mode
    emuone::util::Lock _(_nativeThread->kernel->kernelGuard);
    _nativeThread->state = Thread::State::Finished;
    _nativeThread->exitCode = exitCode;
    //  Lose the NativeThread's "affinity"
    for (auto e : Executors(_nativeThread->affinity))   //  Shallow clone
    {
        Q_ASSERT(e->affineThreads.contains(_nativeThread));
        Q_ASSERT(e->referenceCount > 0);
        Q_ASSERT(_nativeThread->referenceCount > 0);
        e->affineThreads.remove(_nativeThread);
        e->referenceCount--;
        _nativeThread->referenceCount--;
    }
    _nativeThread->affinity.clear();
    //  If all non-daemon Threada of a Process have
    //  Finished, the Process itself has Finished and
    //  the remaining daemon Threads must be killed
    if (_nativeThread->kernel->isReadyToDie(_nativeThread->process))
    {
        _nativeThread->process->state = Process::State::Finished;
        //  TODO finish the implementation - kill daemon threads,
        //  close handles, etc.
    }
}

//  End of emuone-hades/kernel/NativeThread.cpp
