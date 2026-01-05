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
    class EMUONE_HADES_PUBLIC Process
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Process)

        friend class Kernel;
        friend class NativeProcess;
        friend class Thread;
        friend class NativeThread;

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
        Process(Kernel * kernel, const Oid & oid, Identity * owner,
                ExecutionEnvironment * executionEnvironment,
                Process * parent,
                PriorityClass priorityClass,
                const QString & name,
                const QString & command,
                const QString & commandLine,
                const QString & currentDirectory
            );
        virtual ~Process();

        //////////
        //  Properties
        State           state = State::Created;
        PriorityClass   priorityClass;
        QString         name;               //  process name as known to Kernel
        QString         command;            //  the executable that runs the Process
        QString         commandLine;        //  1st [quoted] word is argv[0], etc.
        QString         currentDirectory;   //  "" == not known
        uint32_t        suspendCount = 0;
        std::optional<uint32_t> exitCode;
        bool            reaped = false;     //  somebody did wait() on this Process

        //////////
        //  Associations
        ExecutionEnvironment *  executionEnvironment;   //  counts as "reference"
        Process *       parent;     //  counts as "refrerence", can be nullptr
        Processes       children;   //  count as "references"

        Threads         threads;    //  count as "references"
        Thread *        mainThread = nullptr;   //  counts as "reference"
    };

    /// \class NativeProcess emuone-hades/API.hpp
    /// \brief The Process running native (C++) code.
    class EMUONE_HADES_PUBLIC NativeProcess final
        :   public Process
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeProcess)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        NativeProcess(
                Kernel * kernel, const Oid & oid, Identity * owner,
                NativeExecutionEnvironment * executionEnvironment,
                Process * parent,
                PriorityClass priorityClass,
                const QString & name,
                const QString & command,
                const QString & commandLine,
                const QString & currentDirectory
            );
        virtual ~NativeProcess();
    };
}

//  End of emuone-hades/kernel/Process.hpp
