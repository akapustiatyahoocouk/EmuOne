//
//  emuone-hades/kernel/Oid.cpp - emuone::hades::kernel::Oid class implementation
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
//  Constants
const Oid Oid::Invalid(0x00000000);
const Oid Oid::MinFixedOid(0x00000001);
const Oid Oid::MaxFixedOid(0x0000FFFF);
const Oid Oid::MinRandomOid(0x00010000);
const Oid Oid::MaxRandomOid(0x7FFFFFFF);

//  End of emuone-hades/kernel/Oid.cpp
