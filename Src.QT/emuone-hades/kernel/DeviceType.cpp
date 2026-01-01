//
//  emuone-hades/kernel/DeviceType.cpp - emuone::hades::kernel::DeviceType class implementation
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
using namespace emuone::hades::kernel;

//////////
//  Construction/destruction
DeviceType::DeviceType(Kernel * kernel, const Oid & oid, Identity * owner,
                       uint16_t deviceTypeId_, const QString & name_)
    :   Object(kernel, oid, owner),
        deviceTypeId(deviceTypeId_),
        name(name_)
{
}

DeviceType::~DeviceType()
{
}

//  End of emuone-hades/kernel/DeviceType.cpp
