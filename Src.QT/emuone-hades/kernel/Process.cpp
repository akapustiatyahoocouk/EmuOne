//
//  emuone-hades/kernel/Process.cpp - emuone::hades::kernel::Process class implementation
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
Process::Process(
        Kernel * kernel, const Oid & oid, Identity * owner,
        ExecutionEnvironment * executionEnvironment,
        Process * parent,
        PriorityClass priorityClass,
        const QString & name,
        const QString & command,
        const QString & commandLine,
        const QString & currentDirectory
    ) : Object(kernel, oid, owner),
        //  Properties
        priorityClass(priorityClass),
        name(name),
        command(command),
        commandLine(commandLine),
        currentDirectory(currentDirectory),
        //  Associations
        executionEnvironment(executionEnvironment),
        parent(parent)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(executionEnvironment != nullptr &&
             executionEnvironment->kernel == kernel &&
             kernel->_objects.value(executionEnvironment->oid, nullptr) == executionEnvironment);
    Q_ASSERT((parent == nullptr) ||
             (parent->kernel == kernel &&
             kernel->_objects.value(parent->oid, nullptr) == parent));

    //  Link Process to ExecutionEvironment
    executionEnvironment->processes.insert(this);
    executionEnvironment->referenceCount++;
    this->referenceCount++;

    //  Link Process with parent (if there IS one)
    if (parent != nullptr)
    {
        parent->children.insert(this);
        parent->referenceCount++;
        this->referenceCount++;
    }

    //  Add to secondary Kernel caches
    Q_ASSERT(!kernel->_processes.contains(oid));
    kernel->_processes[oid] = this;
}

Process::~Process()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        Q_ASSERT(children.isEmpty());   //  Children mjst die first!
        //  Un-link Process from ExecutionEnvironment
        Q_ASSERT(executionEnvironment->processes.contains(this));
        executionEnvironment->processes.remove(this);
        Q_ASSERT(executionEnvironment->referenceCount > 0);
        executionEnvironment->referenceCount--;
        Q_ASSERT(this->referenceCount > 0);
        this->referenceCount--;
        //  Un-link Process from parent (if there IS one)
        if (parent != nullptr)
        {
            parent->children.remove(this);
            Q_ASSERT(parent->referenceCount > 0);
            parent->referenceCount--;
            Q_ASSERT(this->referenceCount > 0);
            this->referenceCount--;
        }
    }

    //  Remove from secondary Kernel caches
    Q_ASSERT(kernel->_processes.value(oid, nullptr) == this);
    kernel->_processes.remove(oid);
}

//  End of emuone-hades/kernel/Process.cpp
