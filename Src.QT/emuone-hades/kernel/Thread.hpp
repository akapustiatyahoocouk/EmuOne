//
//  emuone-hades/kernel/Thread.hpp - The HADES OS kernel Thread
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
    /// \class Thread emuone-hades/API.hpp
    /// \brief The Kernel's Thread; tid_t == Oid.
    class EMUONE_HADES_PUBLIC Thread final
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Thread)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Types
        enum class State
        {
            Created,
            Ready,
            Running,
            Waiting,
            Suspended,
            SuspendedWaiting,
            Finished
        };

        //////////
        //  Construction/destruction
        Thread(Kernel * kernel, const Oid & oid, Identity * owner);
        virtual ~Thread();

        //////////
        //  Properties
        State           state = State::Created;
        PriorityClass   priorityClass;
        int             priority;       //  determined from priorityClass
        int             priorityBoost;  //  +/- a bit
        QString         name;           //  "" == none
        std::optional<uint32_t> exitCode;

        //////////
        //  Associations
        Process *       process;    //  never nullptr
    };
}

//  End of emuone-hades/kernel/Thread.hpp
