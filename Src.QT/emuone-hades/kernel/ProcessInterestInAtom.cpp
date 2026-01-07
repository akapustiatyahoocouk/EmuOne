//
//  emuone-hades/kernel/ProcessInterestInAtom.cpp - emuone::hades::kernel::ProcessInterestInAtom class implementation
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
ProcessInterestInAtom::ProcessInterestInAtom(
        Process * process,
        Atom * atom
    ) : process(process),
        atom(atom)
{
    Q_ASSERT(process != nullptr &&
             atom != nullptr &&
             process->kernel == atom->kernel);
    Q_ASSERT(!process->interestingAtoms.contains(atom));
    Q_ASSERT(!atom->interestedProcesses.contains(process));

    process->interestingAtoms[atom] = this;
    atom->interestedProcesses[process] = this;
    process->referenceCount++;
    atom->referenceCount++;
    count = 1;
}

ProcessInterestInAtom::~ProcessInterestInAtom()
{
    Q_ASSERT(process->kernel->kernelGuard.isLockedByCurrentThread());

    if (!process->kernel->_shutdownInProgress)
    {   //  On shutdown everything will be force-destroyed
        Q_ASSERT(process->interestingAtoms.value(atom, nullptr) == this);
        Q_ASSERT(atom->interestedProcesses.value(process, nullptr) == this);
        Q_ASSERT(count == 0);
        process->interestingAtoms.remove(atom);
        atom->interestedProcesses.remove(process);
        process->referenceCount--;
        atom->referenceCount--;
    }
}

//  End of emuone-hades/kernel/ProcessInterestInAtom.cpp
