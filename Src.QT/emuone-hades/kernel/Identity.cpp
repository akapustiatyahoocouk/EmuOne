//
//  emuone-hades/kernel/Identity.cpp - emuone::hades::kernel::Identity class implementation
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
Identity::Identity(
        Kernel * kernel, const Oid & oid,
        const QString & name
    ) : Object(kernel, oid, SelfOwner),
        //  Properties
        name(name)
{
    //  Add to kernel secondary cache(s)
    //  All of them do not count as "references"
    Q_ASSERT(!kernel->_identities.contains(name));
    kernel->_identities[name] = this;
}

Identity::~Identity()
{
    //  Remove from kernel secondary cache(s)
    //  All of them do not count as "references"
    Q_ASSERT(kernel->_identities.value(name, nullptr) == this);
    kernel->_identities.remove(name);
}

//  End of emuone-hades/kernel/Identity.cpp
