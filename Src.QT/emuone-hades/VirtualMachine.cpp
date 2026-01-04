//
//  emuone-hades/VirtualMachine.cpp - emuone::hades::VirtualMachine class implementation
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
using namespace emuone::hades;

//////////
//  Construction/destruction
VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        emuone::core::IVirtualMachineType * type
    ) : emuone::core::VirtualMachine(
            name,
            location,
            Architecture::instance(),
            type)
{
}

VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        emuone::core::IVirtualMachineTemplate * createdFrom
    ) : emuone::core::VirtualMachine(
            name,
            location,
            createdFrom)
{
}

//////////
//  emuone:4:core::VirtualMachine`  (configuration)
bool VirtualMachine::isConfigurationValid() const
{
    if (!emuone::core::VirtualMachine::isConfigurationValid())
    {   //  No point in proceeding
        return false;
    }

    //  A HADES VM must have exactly one HADES OS Kernel
    if (componentsOfType<kernel::Kernel>().size() != 1)
    {   //  OOPS! No Kernel or multiple Kernels
        return false;
    }

    //  All external file systems must specify different and
    //  valid volume names and valid hst paths
    const auto externalFileSystems = componentsOfType<devices::ExternalFileSystem>();
    for (auto a : externalFileSystems)
    {
        for (auto b : externalFileSystems)
        {
            if (a != b && a->volumeName() == b->volumeName())
            {   //  OOPS!
                return false;
            }
        }
    }

    //  All TextTerminals must have different TerminalNo's
    const auto textTerminals = componentsOfType<devices::TextTerminal>();
    for (auto a : textTerminals)
    {
        for (auto b : textTerminals)
        {
            if (a != b && a->terminalNumber() == b->terminalNumber())
            {   //  OOPS!
                return false;
            }
        }
    }

    //  All checks passed
    return true;
}

//  End of emuone-hades/VirtualMachine.cpp
