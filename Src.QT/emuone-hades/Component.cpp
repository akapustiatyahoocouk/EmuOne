//
//  emuone-hades/Component.cpp - emuone::hades::Component class implementation
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
//  Registration
EMUONE_IMPLEMENT_COMPONENT(Component)

//////////
//  emuone::util::IStockObject
QString Component::mnemonic() const
{
    return "emuone::hades";
}

QString Component::displayName() const
{
    return "EmuOne HADES support";
}

//////////
//  emuone::util::IComponent
auto Component::version() const -> QVersionNumber
{
    return emuone::util::fromString<QVersionNumber>(EMUONE_VERSION);
}

QString Component::copyright() const
{
    return "(C) TODO";
}

auto Component::settings() -> Settings *
{
    return Settings::instance();
}

auto Component::settings() const -> const Settings *
{
    return Settings::instance();
}

void Component::iniialize()
{
    //  Register standard stock objects
    emuone::core::ArchitectureManager::register(Architecture::instance());

    emuone::core::ComponentTypeManager::register(devices::ExternalFileSystem::Type::instance());

    emuone::core::ComponentTypeManager::register(kernel::Kernel::Type::instance());
}

void Component::deiniialize()
{
    //  Unregister standard stock objects
    emuone::core::ArchitectureManager::unregister(Architecture::instance());

    emuone::core::ComponentTypeManager::unregister(devices::ExternalFileSystem::Type::instance());

    emuone::core::ComponentTypeManager::unregister(kernel::Kernel::Type::instance());
}

//////////
//  Component::Setting
EMUONE_IMPLEMENT_SINGLETON(Component::Settings)
Component::Settings::Settings() {}
Component::Settings::~Settings() {}

//  End of emuone-hades/Component.cpp
