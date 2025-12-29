//
//  emuone-core/IComponentType.cpp - emuone::core::IComponentType class implementation
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
//  emuone::util::IStockObject
QImage IComponentType::smallImage() const
{
    return category()->smallImage();
}

QImage IComponentType::largeImage() const
{
    return category()->largeImage();
}

//  End of emuone-core/IComponentType.cpp
