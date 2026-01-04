//
//  emuone-hades/kernel/ExecutionEnvironment.cpp - emuone::hades::kernel::ExecutionEnvironment class implementation
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
ExecutionEnvironment::ExecutionEnvironment(
        Kernel * kernel, const Oid & oid, Identity * owner,
        const QString & name, const emuone::util::ByteOrder byteOrder,
        const Executors & affinity
    ) : Object(kernel, oid, owner),
        name(name),
        byteOrder(byteOrder),
        affinity(affinity)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(!affinity.isEmpty());
    for (auto e : affinity)
    {
        Q_ASSERT(e->byteOrder() == byteOrder);
    }

    //  Add to Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(!kernel->_executionEnvironments.contains(name));
    kernel->_executionEnvironments[name] = this;

    //  Link ExecutionEnvironment to Executors
    for (auto e : affinity)
    {
        e->executionEnvironments.insert(this);
        e->referenceCount++;
        this->referenceCount++;
    }
}

ExecutionEnvironment::~ExecutionEnvironment()
{
    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  All Proxcesses must die first!
        Q_ASSERT(processes.isEmpty());
        //  Un-link ExecutionEnvironment from Executors
        for (auto e : affinity)
        {
            Q_ASSERT(e->executionEnvironments.contains(this));
            Q_ASSERT(e->referenceCount > 0);
            Q_ASSERT(this->referenceCount> 0);
            e->executionEnvironments.remove(this);
            e->referenceCount++;
            this->referenceCount++;
        }
    }

    //  Remove Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_executionEnvironments.value(name, nullptr) == this);
    kernel->_executionEnvironments.remove(name);
}

//  End of emuone-hades/kernel/ExecutionEnvironment.cpp
