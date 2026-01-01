//
//  emuone-hades/kernel/Executor.hpp - The HADES OS Executor
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
    /// \class Executor emuone-hades/API.hpp
    /// \brief A generic agent on which code can tun.
    class EMUONE_HADES_PUBLIC Executor
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Executor)

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        Executor(Kernel * kernel, const Oid & oid);
        virtual ~Executor();

        //////////
        //  Associations

        //////////
        //  Operations
        virtual QString name() const = 0;
        virtual auto    byteOrder() const -> emuone::util::ByteOrder;
    };

    /// \class NativeExecutor emuone-hades/API.hpp
    /// \brief The actual processor core.
    class EMUONE_HADES_PUBLIC NativeExecutor final
        :   public Executor
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeExecutor)

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        NativeExecutor(Kernel * kernel, const Oid & oid, Identity * owner,
                       ProcessorCore * processorCore);
        virtual ~NativeExecutor();

        //////////
        //  Executor
        virtual QString name() const override;
        virtual auto    byteOrder() const -> emuone::util::ByteOrder override;

        //////////
        //  Associations
        ProcessorCore * processorCore;  //  counts as "reference", never nullptr
    };
}

//  End of emuone-hades/kernel/Executor.hpp
