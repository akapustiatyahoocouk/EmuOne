//
//  emuone-hades/ISystemProcess.cpp - emuone::hades::ISystemProcess class implementation
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
//  emuone::util::IStockObject
QString ISystemProcess::mnemonic() const
{
    return this->virtualPath();
}

QString ISystemProcess::displayName() const
{
    return this->virtualPath();
}

QIcon ISystemProcess::smallIcon() const
{
    return core::StandardComponentCategories::Software::instance()->smallIcon();
}

QIcon ISystemProcess::largeIcon() const
{
    return core::StandardComponentCategories::Software::instance()->largeIcon();
}

//  End of emuone-hades/ISystemProcess.cpp
