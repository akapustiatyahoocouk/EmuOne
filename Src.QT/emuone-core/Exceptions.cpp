//
//  emuone-core/Exceptions.cpp - VM exceptions
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
//  VirtualMachineCorruptException
VirtualMachineCorruptException::VirtualMachineCorruptException(
        const QString & location
    ) : _location(location)
{
}

QString VirtualMachineCorruptException::errorMessage() const
{
    return "Virtual machine " + _location + " is invalid or corrupt";
}

//////////
//  InvalidVirtualMachineStateException
InvalidVirtualMachineStateException::InvalidVirtualMachineStateException()
{
}

QString InvalidVirtualMachineStateException::errorMessage() const
{
    return "The virtual machine state is invalid";
}

//////////
//  InvalidComponentStateException
InvalidComponentStateException::InvalidComponentStateException()
{
}

QString InvalidComponentStateException::errorMessage() const
{
    return "The virtual machine component state is invalid";
}

//////////
//  IncompatibleComponentException
IncompatibleComponentException::IncompatibleComponentException(
        const QString & componentTypeMnemonic,
        const QString & componentTypeDisplayName
    ) : _componentTypeMnemonic(componentTypeMnemonic),
        _componentTypeDisplayName(componentTypeDisplayName)
{
}

IncompatibleComponentException::IncompatibleComponentException(
        IComponent * component
    ) : IncompatibleComponentException(
            component->type()->mnemonic(),
            component->type()->displayName())
{
}

QString IncompatibleComponentException::errorMessage() const
{
    return "Incompatible " + _componentTypeDisplayName;
}

//////////
//  CustomVirtualMachineException
CustomVirtualMachineException::CustomVirtualMachineException(
        const QString & errorMessage
    ) : _errorMessage(errorMessage)
{
}

QString CustomVirtualMachineException::errorMessage() const
{
    return _errorMessage;
}

//  End of emuone-core/Exceptions.cpp
