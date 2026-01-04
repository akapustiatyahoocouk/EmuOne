//
//  emuone-hades/kernel/NativeExecutor.cpp - emuone::hades::kernel::NativeExecutor class implementation
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
NativeExecutor::NativeExecutor(
        Kernel * kernel, const Oid & oid, Identity * owner,
        ProcessorCore * processorCore
    ) : Executor(kernel, oid, owner, processorCore->name),
        processorCore(processorCore)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(processorCore != nullptr);
    Q_ASSERT(processorCore != nullptr &&
             processorCore->kernel == kernel &&
             kernel->_objects.value(processorCore->oid, nullptr) == processorCore);

    //  Link Executor with ProcessorCore
    Q_ASSERT(processorCore->executor == nullptr);
    processorCore->executor = this;
    this->referenceCount++;
    processorCore->referenceCount++;
}

NativeExecutor::~NativeExecutor()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    if (!kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        //  Uk-link Executor from ProcessorCore
        Q_ASSERT(processorCore != nullptr &&
                 processorCore->executor == this);
        Q_ASSERT(this->referenceCount > 0);
        Q_ASSERT(processorCore->executor->referenceCount > 0);
        processorCore->executor = nullptr;
        this->referenceCount--;
        processorCore->referenceCount--;
    }
}

//////////
//  Executor
auto NativeExecutor::byteOrder() const -> emuone::util::ByteOrder
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    return processorCore->byteOrder;
}

//  End of emuone-hades/kernel/NativeExecutor.cpp
