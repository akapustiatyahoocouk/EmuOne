//
//  emuone-hades/kernel/Kernel.IdentityManager.cpp - Identity manager
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
//  Operations (identity management)
SystemIdentity * Kernel::createSystemIdentity()
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(_systemIdentity == nullptr);   //  One only per Kernel!

    auto systemIdentity = new SystemIdentity(this, Oid(1));
    Q_ASSERT(systemIdentity == _systemIdentity);
    Q_ASSERT(_objects.value(systemIdentity->oid, nullptr) == systemIdentity);
    return systemIdentity;
}

//  End of emuone-hades/kernel/Kernel.IdentityManager.cpp
