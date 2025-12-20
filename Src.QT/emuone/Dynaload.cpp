//
//  emuone/Dynaload.cpp - Dynamic loading support
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
#include "emuone/API.hpp"
using namespace emuone;

//////////
//  Singleton
EMUONE_IMPLEMENT_COMPONENT(Component)

//////////
//  emuone::util::IStockObject
QString Component::mnemonic() const
{
    return "emuone";
}

QString Component::displayName() const
{
    return "EmuOne";
}

QString Component::copyright() const
{
    return "(C) TODO";
}

//////////
//  emuone::util::IComponent
auto Component::version() const -> QVersionNumber
{
    return emuone::util::fromString<QVersionNumber>(EMUONE_VERSION);
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
}

void Component::deiniialize()
{
}

//////////
//  Component::Setting
EMUONE_IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   mainFrameBounds(this, "MainFrameBounds", QRect(32, 32, 480, 320)),
        mainFrameMaximized(this, "MainFrameMaximized", false)
{
}

Component::Settings::~Settings()
{
}

//  End of emuone/Dynaload.cpp
