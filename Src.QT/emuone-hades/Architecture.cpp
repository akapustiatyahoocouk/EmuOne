//
//  emuone-hades/Architecture.cpp - emuone::hades::Architecture class implementation
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
//  Singleton
EMUONE_IMPLEMENT_SINGLETON(Architecture)
Architecture::Architecture() {}
Architecture::~Architecture() {}

//////////
//  emuone::util::IStockObject
QString Architecture::mnemonic() const
{
    return "HADES";
}

QString Architecture::displayName() const
{
    return "HADES VM";
}

QIcon Architecture::smallIcon() const
{
    static const QIcon icon(":/emuone-hades/Resources/Images/Objects/HadesSmall.png");
    return icon;
}

QIcon Architecture::largeIcon() const
{
    static const QIcon icon(":/emuone-hades/Resources/Images/Objects/HadesLarge.png");
    return icon;
}

//////////
//  emuone::core::IArchitecture
auto Architecture::createVirtualMachine(
        const QString & name,
        const QString & location,
        emuone::core::IVirtualMachineType * type
    ) -> emuone::core::VirtualMachine *
{
    return new VirtualMachine(name, location, type);
}

//  End of emuone-hades/Architecture.cpp
