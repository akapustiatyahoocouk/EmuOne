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
Thread::Thread(
        Kernel * kernel, const Oid & oid, Identity * owner,
        Process * process,
        PriorityClass priorityClass,
        const QString name,
        const Executors & affinity
    ) : Object(kernel, oid, owner),
        //  Properties
        priorityClass(priorityClass),
        priority(prioriryFromClass(priorityClass)),
        name(name),
        //  Associations
        process(process),
        affinity(affinity)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr &&
             process->kernel == kernel &&
             kernel->_objects.value(process->oid, nullptr) == process);
    Q_ASSERT(process->executionEnvironment->affinity.contains(affinity) &&
             !affinity.isEmpty());

    //  Link Thread to the Process
    process->threads.insert(this);
    process->referenceCount++;
    this->referenceCount++;

    //  Link Thread to Executors
    for (auto e : affinity)
    {
        e->affineThreads.insert(this);
        e->referenceCount++;
        this->referenceCount++;
    }
}

Thread::~Thread()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  Link Thread to the Process
        Q_ASSERT(process->threads.contains(this));
        Q_ASSERT(process->referenceCount > 0);
        Q_ASSERT(this->referenceCount > 0);
        process->threads.remove(this);
        process->referenceCount--;
        this->referenceCount--;
        //  Un-link Thread from Executors
        for (auto e : affinity)
        {
            Q_ASSERT(e->affineThreads.contains(this));
            Q_ASSERT(e->referenceCount > 0);
            Q_ASSERT(this->referenceCount > 0);
            e->affineThreads.remove(this);
            e->referenceCount--;
            this->referenceCount--;
        }
    }
}

//////////
//  Operations
int Thread::prioriryFromClass(PriorityClass priorityClass)
{
    switch (priorityClass)
    {
        case PriorityClass::Idle:
            return Priority::Idle;
        case PriorityClass::LowPriority:
            return Priority::LowPrioroty;
        case PriorityClass::Normal:
            return Priority::NormalPriority;
        case PriorityClass::HighPriority:
            return Priority::HighPriority;
        case PriorityClass::TimeCritical:
            return Priority::TimeCritical;
        case PriorityClass::Realtime:
            return Priority::RealTime;
        default:
            Q_ASSERT(false);
            //  Be defensive in Release mode
            return Priority::NormalPriority;
    }
}

//  End of emuone-hades/kernel/Thread.cpp

