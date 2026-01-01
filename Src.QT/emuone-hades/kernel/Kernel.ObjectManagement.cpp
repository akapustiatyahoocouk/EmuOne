//
//  emuone-hades/kernel/Kernel.ObjectManagement.cpp - Object management
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
//  Operations (object management)
Oid Kernel::generateUnusedOid()
{
    uint32_t spread = Oid::MaxRandomOid._impl - Oid::MinRandomOid._impl + 1;
    for (; ; )
    {   //  Will succeed eventually
        uint32_t impl =
            QRandomGenerator::global()->bounded(spread) +
            Oid::MinRandomOid._impl;
        if (Oid oid = Oid(impl); !_objects.contains(oid))
        {   //  Use this one
            return oid;
        }
    }
}

//  End of emuone-hades/kernel/Kernel.ObjectManagement.cpp
