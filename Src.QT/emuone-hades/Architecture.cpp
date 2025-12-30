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
bool Architecture::isValid(
        emuone::core::VirtualMachine * virtualMachine
    ) const
{
    Q_ASSERT(virtualMachine != nullptr);

    if (virtualMachine->architecture() != this)
    {   //  OOPS!
        return false;
    }
    //  A HADES VM must have exactly one HADES OS Kernel
    if (virtualMachine->componentsOfType<kernel::Kernel>().size() != 1)
    {   //  OOPS! No Kernel or multiple Kernels
        return false;
    }
    //  All external file systems must specify different and
    //  valid volume names and valid hst paths
    QSet<QString> volumeNames;
    for (auto efs : virtualMachine->componentsOfType<devices::ExternalFileSystem>())
    {
        if (!devices::ExternalFileSystem::isValidVolumeName(efs->volumeName()) ||
            !devices::ExternalFileSystem::isValidHostPath(efs->hostPath()))
        {   //  OOPS!
            return false;
        }
        if (volumeNames.contains(efs->volumeName()))
        {   //  OOPS!
            return false;
        }
        volumeNames.insert(efs->volumeName());
    }
    //  All checks passed
    return true;

}

//  End of emuone-hades/Architecture.cpp
