//
//  emuone-hades/Classes.hpp - Forward declarations and typedef
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

namespace emuone::hades::kernel
{
    //  Objects
    class Kernel;
    class Object;
    class Identity;
    class SystemIdentity;
    class UserIdentity;
    class SyntheticIdentity;

    class Executor;
    class NativeExecutor;
    class EmulatedExecutor;
    class ExecutionEnvironment;
    class NativeExecutionEnvironment;

    class DeviceType;
    class Device;
    class Processor;
    class ProcessorCore;

    //  Pointers (used as output parameters in Kernel services)
    using PDeviceType = DeviceType*;
    using PDevice = Device*;
    using PProcessor = Processor*;
    using PProcessorCore = ProcessorCore*;

    //  Collections
    using Objects = QSet<Object*>;
    using Devices = QSet<Device*>;
}

//  End of emuone-hades/Classes.hpp
