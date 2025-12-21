//
//  emuone-core/VirtualMachine.cpp - emuone::core::VirtualMachine class implementation
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
//  Construction/destruction
VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        IArchitecture * architecture,
        IVirtualMachineType * virtualMachineType,
        ITemplate * vmTemplate
    ) : _name(name.trimmed()),  //  be defensive!
        _location(QFileInfo(location).absoluteFilePath()),  //  be defensive!
        _architecture(architecture),
        _type(virtualMachineType),
        _template(vmTemplate)
{
    Q_ASSERT(isValidName(_name)),
    Q_ASSERT(_architecture != nullptr);
    Q_ASSERT(_type != nullptr);
    Q_ASSERT( _template == nullptr ||
             (_template->architecture() == _architecture &&
              _template->virtualMachineType() == _type));
}

VirtualMachine::~VirtualMachine()
{
    //  TODO stop!
}

//////////
//  Operations (general)
QString VirtualMachine::name() const
{
    emuone::util::Lock _(_guard);
    return _name;
}

void VirtualMachine::setName(const QString & name)
{
    emuone::util::Lock _(_guard);

    Q_ASSERT(isValidName(name));
    if (isValidName(name))
    {   //  Be defensive in release mode
        _name = name;
    }
}

bool VirtualMachine::isValidName(const QString & name)
{
    return name.length() > 0 &&
           name.length() <= 128 &&
           name.trimmed().length() == name.length() &&
           std::all_of(
            name.cbegin(),
            name.cend(),
            [](auto c)
            {
                return c.unicode() >= 32 && c.unicode() != 127;
            });
}

QString VirtualMachine::location() const
{
    emuone::util::Lock _(_guard);

    return _location;
}

QString VirtualMachine::savedStateFileName() const
{
    emuone::util::Lock _(_guard);
    return _location + SavedStateExtension;
}

IArchitecture * VirtualMachine::architecture() const
{
    emuone::util::Lock _(_guard);
    return _architecture;
}

IVirtualMachineType * VirtualMachine::virtualMachineType() const
{
    emuone::util::Lock _(_guard);
    return _type;
}

ITemplate * VirtualMachine::createdFrom() const
{
    emuone::util::Lock _(_guard);
    return _template;
}

//  End of emuone-core/VirtualMachine.cpp
