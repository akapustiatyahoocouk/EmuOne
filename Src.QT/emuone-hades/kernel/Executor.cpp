//
//  emuone-hades/kernel/Executor.cpp - emuone::hades::kernel::Executor class implementation
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
Executor::Executor(
        Kernel * kernel, const Oid & oid, Identity * owner,
        const QString & name
    ) : Object(kernel, oid, owner),
        name(name)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Add to Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(!kernel->_executors.contains(name));
    kernel->_executors[name] = this;
}

Executor::~Executor()
{
    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  Execution environments must die first!
        Q_ASSERT(executionEnvironments.isEmpty());
        //  Affine threads must die first
        Q_ASSERT(affineThreads.isEmpty());
    }

    //  Remove from Kernel's secondary caches
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_executors.value(name, nullptr) == this);
    kernel->_executors.remove(name);
}

//  End of emuone-hades/kernel/Executor.cpp
