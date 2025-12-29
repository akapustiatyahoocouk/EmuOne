//
//  emuone-util/IComponent.cpp - emuone::util::IComponent class implementation
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
#include "emuone-util/API.hpp"
using namespace emuone::util;

//////////
//  IStockObject
QIcon IComponent::smallIcon() const
{
    static const QIcon icon(":/emuone-util/Resources/Images/Misc/ComponentSmall.png");
    return icon;
}

QIcon IComponent::largeIcon() const
{
    static const QIcon icon(":/emuone-util/Resources/Images/Misc/ComponentLarge.png");
    return icon;
}

//  End of emuone-util/IComponent.cpp
