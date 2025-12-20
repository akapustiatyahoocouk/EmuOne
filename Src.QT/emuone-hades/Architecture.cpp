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

QString Architecture::copyright() const
{
    return "(C) TODO";
}

QImage Architecture::smallImage() const
{
    static const QImage image(":/emuone-hades/Resources/Images/Objects/HadesSmall.png");
    return image;
}

QImage Architecture::largeImage() const
{
    static const QImage image(":/emuone-hades/Resources/Images/Objects/HadesLarge.png");
    return image;
}

//  End of emuone-hades/Architecture.cpp
