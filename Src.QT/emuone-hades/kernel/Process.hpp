//
//  emuone-hades/kernel/Process.hpp - The HADES OS kernel Process
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
    /// \class Process emuone-hades/API.hpp
    /// \brief The Kernel's Process; pid_t == Oid.
    class EMUONE_HADES_PUBLIC Process final
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Process)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Types
        enum class State
        {
            Created,
            Running,
            Suspended,
            Finished
        };

        //////////
        //  Construction/destruction
        Process(Kernel * kernel, const Oid & oid, Identity * owner);
        virtual ~Process();

        //////////
        //  Properties
        State           state = State::Created;
        PriorityClass   priorityClass;
        QString         name;
        QString         command;
        QString         commandLine;
        QString         currentDirectory;   //  "" == not known
        std::optional<uint32_t> exitCode;

        //////////
        //  Associations
        Process *       parent;     //  counts as "refrerence", can be nullptr
        Processes       children;   //  count as "references"

        Threads         threads;
    };
}

//  End of emuone-hades/kernel/Process.hpp
