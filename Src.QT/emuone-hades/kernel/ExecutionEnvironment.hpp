//
//  emuone-hades/kernel/ExecutionEnvironment.hpp - The HADES OS Execution Environment
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
    /// \class ExecutionEnvironment emuone-hades/API.hpp
    /// \brief A generic environment where pricesses can run.
    class EMUONE_HADES_PUBLIC ExecutionEnvironment
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ExecutionEnvironment)

        friend class Kernel;
        friend class NativeExecutionEnvironment;
        friend class Process;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        ExecutionEnvironment(
                Kernel * kernel, const Oid & oid, Identity * owner,
                const QString & name, const emuone::util::ByteOrder byteOrder,
                const Executors & affinity
            );
        virtual ~ExecutionEnvironment();

        //////////
        //  Properties
        const QString   name;
        const emuone::util::ByteOrder   byteOrder;

        //////////
        //  Associations
        Executors       affinity;   //  count as "references"
        Processes       processes;
    };

    /// \class NativeExecutionEnvironment emuone-hades/API.hpp
    /// \brief A native HADES execution on underlying hardware.
    class EMUONE_HADES_PUBLIC NativeExecutionEnvironment
        :   public ExecutionEnvironment
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeExecutionEnvironment)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        NativeExecutionEnvironment(
                Kernel * kernel, const Oid & oid, Identity * owner,
                const QString & name,
                const NativeExecutors & affinity
            );
        virtual ~NativeExecutionEnvironment();
    };
}

//  End of emuone-hades/kernel/ExecutionEnvironment.hpp
