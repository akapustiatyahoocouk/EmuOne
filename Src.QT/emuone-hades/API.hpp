//
//  emuone-hades/API.hpp - emuone-hades master header
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
#pragma once

//////////
//  Dependencies
#include "emuone-core/API.hpp"
#include "emuone-util/API.hpp"

//////////
//  emuone-hades components
#include "emuone-hades/Linkage.hpp"
#include "emuone-hades/Classes.hpp"
#include "emuone-hades/Component.hpp"

//  Stock objects
#include "emuone-hades/Architecture.hpp"

//  Devices
#include "emuone-hades/devices/ExternalFileSystem.hpp"
#include "emuone-hades/devices/TextTerminal.hpp"

//  HADES Kernel
#include "emuone-hades/kernel/DataTypes.hpp"
#include "emuone-hades/kernel/Kernel.hpp"
#include "emuone-hades/kernel/Object.hpp"
#include "emuone-hades/kernel/Identity.hpp"
#include "emuone-hades/kernel/Executor.hpp"
#include "emuone-hades/kernel/ExecutionEnvironment.hpp"
#include "emuone-hades/kernel/DeviceType.hpp"
#include "emuone-hades/kernel/Device.hpp"
#include "emuone-hades/kernel/Process.hpp"
#include "emuone-hades/kernel/Thread.hpp"

//  Processes
#include "emuone-hades/processes/init/InitProcess.hpp"

//  Miscellaneous
#include "emuone-hades/VirtualMachine.hpp"

//  End of emuone-hades/API.hpp
