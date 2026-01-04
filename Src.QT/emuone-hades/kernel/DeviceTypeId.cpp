//
//  emuone-hades/kernel/DeviceTypeId.cpp - emuone::hades::kernel::DeviceTypeId class implementation
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
const DeviceTypeId  DeviceTypeId::Amd64Processor{0x00000100};
const DeviceTypeId  DeviceTypeId::Amd64ProcessorCore{0x00000101};
#if defined(Q_PROCESSOR_X86_64)
    const DeviceTypeId  DeviceTypeId::HostProcessor{0x00000100};
    const DeviceTypeId  DeviceTypeId::HostProcessorCore{0x00000101};
#else
    #error Unsupported host CPU
#endif

//  End of emuone-hades/kernel/DeviceTypeId.cpp
