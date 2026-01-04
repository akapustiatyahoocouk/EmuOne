//
//  emuone-hades/kernel/NativeExecutionEnvironment.cpp - emuone::hades::kernel::NativeExecutionEnvironment class implementation
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
NativeExecutionEnvironment::NativeExecutionEnvironment(
        Kernel * kernel, const Oid & oid, Identity * owner,
        const QString & name,
        const NativeExecutors & affinity
    ) : ExecutionEnvironment(
              kernel, oid, owner,
              name,
              emuone::util::ByteOrder::Host,
              Executors(affinity.cbegin(), affinity.cend()))
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
}

NativeExecutionEnvironment::~NativeExecutionEnvironment()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());
}

//  End of emuone-hades/kernel/NativeExecutionEnvironment.cpp
