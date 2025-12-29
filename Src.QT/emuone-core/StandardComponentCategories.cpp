//
//  emuone-core/StandardComponentCategories.cpp - emuone::core::StandardComponentCategories class implementation
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
using SCC = StandardComponentCategories;

//////////
//  Operations
auto StandardComponentCategories::all(
    ) -> ComponentCategories
{
    static const ComponentCategories result
    {
        Processors::instance(),
        Memory::instance(),
        Storage::instance(),
        OnboardDevices::instance(),
        UiDevices::instance(),
        Software::instance(),
        Miscellaneous::instance()
    };
    return result;
}

//////////
//  StandardComponentCategories::Processors
EMUONE_IMPLEMENT_SINGLETON(SCC::Processors)
SCC::Processors::Processors() {}
SCC::Processors::~Processors() {}

QString SCC::Processors::mnemonic() const
{
    return "Processors";
}

QString SCC::Processors::displayName() const
{
    return "Processors";
}

QIcon SCC::Processors::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/ProcessorSmall.png");
    return icon;
}

QIcon SCC::Processors::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/ProcessorLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::Memory
EMUONE_IMPLEMENT_SINGLETON(SCC::Memory)
SCC::Memory::Memory() {}
SCC::Memory::~Memory() {}

QString SCC::Memory::mnemonic() const
{
    return "Memory";
}

QString SCC::Memory::displayName() const
{
    return "Memory";
}

QIcon SCC::Memory::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/MemorySmall.png");
    return icon;
}

QIcon SCC::Memory::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/MemoryLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::Storage
EMUONE_IMPLEMENT_SINGLETON(SCC::Storage)
SCC::Storage::Storage() {}
SCC::Storage::~Storage() {}

QString SCC::Storage::mnemonic() const
{
    return "Storage";
}

QString SCC::Storage::displayName() const
{
    return "Storage";
}

QIcon SCC::Storage::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/StorageSmall.png");
    return icon;
}

QIcon SCC::Storage::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/StorageLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::OnboardDevices
EMUONE_IMPLEMENT_SINGLETON(SCC::OnboardDevices)
SCC::OnboardDevices::OnboardDevices() {}
SCC::OnboardDevices::~OnboardDevices() {}

QString SCC::OnboardDevices::mnemonic() const
{
    return "OnboardDevices";
}

QString SCC::OnboardDevices::displayName() const
{
    return "Onboard devices";
}

QIcon SCC::OnboardDevices::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/OnboardDeviceSmall.png");
    return icon;
}

QIcon SCC::OnboardDevices::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/OnboardDeviceLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::UiDevices
EMUONE_IMPLEMENT_SINGLETON(SCC::UiDevices)
SCC::UiDevices::UiDevices() {}
SCC::UiDevices::~UiDevices() {}

QString SCC::UiDevices::mnemonic() const
{
    return "UiDevices";
}

QString SCC::UiDevices::displayName() const
{
    return "UI devices";
}

QIcon SCC::UiDevices::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/UiDeviceSmall.png");
    return icon;
}

QIcon SCC::UiDevices::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/UiDeviceLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::Software
EMUONE_IMPLEMENT_SINGLETON(SCC::Software)
SCC::Software::Software() {}
SCC::Software::~Software() {}

QString SCC::Software::mnemonic() const
{
    return "Software";
}

QString SCC::Software::displayName() const
{
    return "Software";
}

QIcon SCC::Software::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/SoftwareSmall.png");
    return icon;
}

QIcon SCC::Software::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/SoftwareLarge.png");
    return icon;
}

//////////
//  StandardComponentCategories::Miscellaneous
EMUONE_IMPLEMENT_SINGLETON(SCC::Miscellaneous)
SCC::Miscellaneous::Miscellaneous() {}
SCC::Miscellaneous::~Miscellaneous() {}

QString SCC::Miscellaneous::mnemonic() const
{
    return "Miscellaneous";
}

QString SCC::Miscellaneous::displayName() const
{
    return "Miscellaneous";
}

QIcon SCC::Miscellaneous::smallIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/GenericDeviceSmall.png");
    return icon;
}

QIcon SCC::Miscellaneous::largeIcon() const
{
    static const QIcon icon(":/emuone-core/Resources/Images/Objects/GenericDeviceLarge.png");
    return icon;
}

//  End of emuone-core/StandardComponentCategories.cpp
