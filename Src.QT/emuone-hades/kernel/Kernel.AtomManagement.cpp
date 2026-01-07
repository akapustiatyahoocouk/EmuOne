//
//  emuone-hades/kernel/Kernel.AtomManagement.cpp - Atom  management
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
//  Operations (atom management)
KErrno Kernel::getAtom(
        Process * process,
        const QString & value,
        PAtom & atom
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr &&
             process->kernel == this &&
             _objects.value(process->oid, nullptr) == process);
    Q_ASSERT(_systemIdentity != nullptr);

    //  Does the Atom object already exist ?
    atom = _atoms.value(value, nullptr);
    if (atom == nullptr)
    {
        atom = new Atom(
            this,
            generateUnusedOid(),
            _systemIdentity,
            value);
    }
    Q_ASSERT(atom != nullptr &&
             _atoms.value(value, nullptr) == atom &&
             atom->value == value);
    //  Does the Process already have an Interest in the Atom ?
    if (auto interest = process->interestingAtoms.value(atom, nullptr))
    {   //  Yes - incremenent inrerest count
        if (interest->count + 1 <= interest->count)
        {   //  OOPS! Overflow!
            return K_EOVERFLOW;
        }
        interest->count++;
    }
    else
    {   //  No - must create a new Interest
        interest = new ProcessInterestInAtom(process, atom);
        Q_ASSERT(process->interestingAtoms.value(atom, nullptr) == interest);
        Q_ASSERT(atom->interestedProcesses.value(process, nullptr) == interest);
        Q_ASSERT(interest->count == 0);
        interest->count++;
    }
    return K_EOK;
}

KErrno Kernel::releaseAtom(
        Process * process,
        Atom * atom
    )
{
    Q_ASSERT(kernelGuard.isLockedByCurrentThread());
    Q_ASSERT(process != nullptr &&
             process->kernel == this &&
             _objects.value(process->oid, nullptr) == process);
    Q_ASSERT(atom != nullptr &&
             atom->kernel == this &&
             _objects.value(atom->oid, nullptr) == atom);

    if (auto interest = process->interestingAtoms.value(atom, nullptr))
    {   //  There IS an interest
        Q_ASSERT(interest->count > 0);
        interest->count--;
        //  Drop the interest ?
        if (interest->count == 0)
        {
            delete interest;
            Q_ASSERT(!process->interestingAtoms.contains(atom));
            Q_ASSERT(!atom->interestedProcesses.contains(process));
        }
        return K_EOK;
    }
    //  Else the Process is not interested in Atom
    return K_EINVAL;
}

//  End of emuone-hades/kernel/Kernel.AtomManagement.cpp
