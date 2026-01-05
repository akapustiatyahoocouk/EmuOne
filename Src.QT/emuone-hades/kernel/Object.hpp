//
//  emuone-hades/kernel/Object.hpp - The HADES OS Kernel object
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
    /// \class Object emuone-hades/API.hpp
    /// \brief The HADES OS Kernel.object.
    class EMUONE_HADES_PUBLIC Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        friend class Kernel;
        friend class Identity;
        friend class SystemIdentity;
        friend class DeviceType;
        friend class Device;
        friend class Processor;
        friend class ProcessorCore;
        friend class Executor;
        friend class NativeExecutor;
        friend class ExecutionEnvironment;
        friend class NativeExecutionEnvironment;
        friend class Process;
        friend class Thread;
        friend class NativeThread;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Constants
        //  A special constant that makes a newly constructed
        //  Object to "own itself"
        static Identity *const SelfOwner;

        //////////
        //  Construction/destruction
        Object(Kernel * kernel, const Oid & oid, Identity * ownerOrSelf);
        virtual ~Object();

        //////////
        //  Properties
        Kernel *const   kernel; //  managing this Object
        const Oid       oid;    //  unique per Kernel

        //  counts all pointers to this object from kernel data structures
        uint32_t        referenceCount = 0;

        //////////
        //  Associations
        Identity *      owner;  //  counts as a reference
    };
}

//  End of emuone-hades/kernel/Object.hpp
