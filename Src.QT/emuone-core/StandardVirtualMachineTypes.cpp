//
//  emuone-core/StandardVirtualMachineTypes.cpp - emuone::core::StandardVirtualMachineTypes class implementation
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
#include "emuone-core/API.hpp"
using namespace emuone::core;

//////////
//  Operations
auto StandardVirtualMachineTypes::all() -> VirtualMachineTypes
{
    static const VirtualMachineTypes result
    {
        VirtualMachine::instance(),
        RemoteTerminal::instance()
    };
    return result;
}

//////////
//  StandardVirtualMachineTypes::VirtualMachine
EMUONE_IMPLEMENT_SINGLETON(StandardVirtualMachineTypes::VirtualMachine)
StandardVirtualMachineTypes::VirtualMachine::VirtualMachine() {}
StandardVirtualMachineTypes::VirtualMachine::~VirtualMachine() {}

QString StandardVirtualMachineTypes::VirtualMachine::mnemonic() const
{
    return "VirtualMachine";
}

QString StandardVirtualMachineTypes::VirtualMachine::displayName() const
{
    return "Virtual Machine";
}

QIcon StandardVirtualMachineTypes::VirtualMachine::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/VirtualMachineSmall.png");
    return icon;
}

QIcon StandardVirtualMachineTypes::VirtualMachine::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/VirtualMachineLarge.png");
    return icon;
}

//////////
//  StandardVirtualMachineTypes::RemoteTerminal
EMUONE_IMPLEMENT_SINGLETON(StandardVirtualMachineTypes::RemoteTerminal)
StandardVirtualMachineTypes::RemoteTerminal::RemoteTerminal() {}
StandardVirtualMachineTypes::RemoteTerminal::~RemoteTerminal() {}

QString StandardVirtualMachineTypes::RemoteTerminal::mnemonic() const
{
    return "RemoteTerminal";
}

QString StandardVirtualMachineTypes::RemoteTerminal::displayName() const
{
    return "Remote Terminal";
}

QIcon StandardVirtualMachineTypes::RemoteTerminal::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/RemoteTerminalSmall.png");
    return icon;
}

QIcon StandardVirtualMachineTypes::RemoteTerminal::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/RemoteTerminalLarge.png");
    return icon;
}

//  End of emuone-core/StandardVirtualMachineTypes.cpp
