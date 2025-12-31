//
//  emuone-hades/kernel/Object.cpp - emuone::hades::kernel::Object class implementation
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
//  Constants
Identity *const Object::SelfOwner = reinterpret_cast<Identity*>(0x1234);

//////////
//  Construction/destruction
Object::Object(Kernel * kernel_, const Oid & oid_, Identity * owner_)
    :   kernel(kernel_),
        oid(oid_),
        owner((owner_ == SelfOwner) ? static_cast<Identity*>(this) : owner)
{
    Q_ASSERT(kernel != nullptr);
    Q_ASSERT(owner != nullptr);
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Add to kernel cache(s)
    Q_ASSERT(!kernel->_objects.contains(oid));
    kernel->_objects[oid] = this;
    referenceCount++;

    //  Set up owner/object links
    if (owner == this)
    {   //  Special case - creating an Identity
        //  Owner and owned object now "refer to" each other
        owner->referenceCount += 2;
    }
    else
    {   //  General case
        owner->ownedObjects.insert(this);
        //  Owner and owned object now "refer to" each other
        owner->referenceCount++;
        this->referenceCount++;
    }
}

Object::~Object()
{
    Q_ASSERT(kernel->kernelGuard.isLockedByCurrentThread());

    //  Break owner/object links
    if (owner == this)
    {   //  Must be a system identity
        //  Owner and owned object no longer "refer to" each other
        Q_ASSERT(this->referenceCount >= 2);
        owner->referenceCount -= 2;
    }
    else
    {   //  Just an object
        Q_ASSERT(owner->ownedObjects.contains(this));
        owner->ownedObjects.remove(this);
        //  Owner and owned object no longer "refer to" each other
        Q_ASSERT(owner->referenceCount > 0);
        Q_ASSERT(this->referenceCount > 0);
        owner->referenceCount--;
        this->referenceCount--;
    }

    //  Remove from to kernel cache(s)
    Q_ASSERT(kernel->_objects.value(oid, nullptr) == this);
    kernel->_objects.remove(oid);
    Q_ASSERT(this->referenceCount == 1);    //  otherwise destruction is an error
    referenceCount--;
}

//  End of emuone-hades/kernel/Object.cpp
