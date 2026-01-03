//
//  emuone-hades/kernel/SystemIdentity.cpp - emuone::hades::kernel::SystemIdentity class implementation
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
SystemIdentity::SystemIdentity(Kernel * kernel, const Oid & oid)
    :   Identity(kernel, oid, Name)
{
    //  Add to kernel secondary cache(s)
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_systemIdentity == nullptr);
    kernel->_systemIdentity = this;
}

SystemIdentity::~SystemIdentity()
{
    //  Remove from kernel's secondary cache(s)
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_systemIdentity == this);
    kernel->_systemIdentity = nullptr;
}

//  End of emuone-hades/kernel/SystemIdentity.cpp
