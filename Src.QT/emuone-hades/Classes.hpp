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

    class Process;
    class Thread;

    //  Pointers (used as output parameters in Kernel services)
    using PSystemIdentity = SystemIdentity*;
    using PUserIdentity = UserIdentity*;
    using PSyntheticIdentity = SyntheticIdentity*;

    using PExecutor = Executor*;
    using PNativeExecutor = NativeExecutor*;
    using PExecutionEnvironment = ExecutionEnvironment*;
    using PNativeExecutionEnvironment = NativeExecutionEnvironment*;

    using PDeviceType = DeviceType*;
    using PDevice = Device*;
    using PProcessor = Processor*;
    using PProcessorCore = ProcessorCore*;

    using PProcess = Process*;
    using PThread = Thread*;

    //  Collections
    using Objects = QSet<Object*>;
    using Executors = QSet<Executor*>;
    using NativeExecutors = QSet<NativeExecutor*>;
    using ExecutionEnvironments = QSet<ExecutionEnvironment*>;
    using Devices = QSet<Device*>;
    using Processors = QSet<Processor*>;
    using ProcessorCores = QSet<ProcessorCore*>;
    using Processes = QSet<Process*>;
    using Threads = QSet<Thread*>;
}

//  End of emuone-hades/Classes.hpp
