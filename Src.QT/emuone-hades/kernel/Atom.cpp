//
//  emuone-hades/kernel/Atom.cpp - emuone::hades::kernel::Atom class implementation
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
Atom::Atom(
        Kernel * kernel, const Oid & oid, Identity * owner,
        const QString & value
    ) : Object(kernel, oid, owner),
        value(value)
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Add to secondary Kernel caches
    Q_ASSERT(!kernel->_atoms.contains(value));
    kernel->_atoms[value] = this;
}

Atom::~Atom()
{
    //  Remove from secondary Kernel caches
    Q_ASSERT(kernel->_atoms.value(value, nullptr) == this);
    kernel->_atoms.remove(value);
}

//  End of emuone-hades/kernel/Atom.cpp
