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
    class EMUONE_HADES_PUBLIC Thread
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Thread)

        friend class Kernel;
        friend class NativeThread;

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

        enum Priority
        {
            //  Overall pthreads priority range
            Min = 0,
            Max = 31,

            //  Idle threads
            Idle = 0,           ///< PriorityClass::Idle

            //  "normal" roiund robin
            MinNormal = 1,
            LowPrioroty = 3,    ///< PriorityClass::LowPriority
            NormalPriority = 8, ///< PriorityClass::Normal
            HighPriority = 13,  ///< PriorityClass::HighPriority
            MaxNormal = 15,

            //  Time-critical threads
            MinTimeCritical = 16,
            TimeCritical = 23,  ///< PriorityClass::TimeCritical
            MaxTimeCritical = 30,

            //  Realtime threads
            RealTime = 31,      ///< PriorityClass::Realtime

            //  The min/max priority "boost" applicable to a Thread.
            MinPriorityBoost = -3,
            MaxPriorityBoost = 3
        };

        //////////
        //  Construction/destruction
        Thread(Kernel * kernel, const Oid & oid, Identity * owner,
               Process * process,
               PriorityClass priorityClass,
               const QString name,
               const Executors & affinity
            );
        virtual ~Thread();

        //////////
        //  Properties
        State           state = State::Created;
        PriorityClass   priorityClass;
        int             priority;           //  determined from priorityClass
        int             priorityBoost = 0;  //  +/- a little bit, never applied to Realtime threads
        QString         name;               //  "" == none
        bool            isDaemon = false;   //  always starts off as non-daemon
        uint32_t        suspendCount = 0;
        std::optional<uint32_t> exitCode;
        bool            reaped = false;     //  somebody did wait() on this Thread

        //////////
        //  Associations
        Process *       process;    //  never nullptr
        Executors       affinity;   //  count as "references"

        //////////
        //  Operations
        int             dynamicPriority() const
        {
            return std::max(int(Min),
                            std::min(int(Max), priority + priorityBoost));
        }

        static int      prioriryFromClass(PriorityClass priorityClass);
    };

    /// \class NativeThreadRunner emuone-hades/API.hpp
    /// \brief An agent that implements a NativeThread.
    class EMUONE_HADES_PUBLIC NativeThreadRunner
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeThreadRunner)

        friend class Kernel;
        friend class NativeThread;

        //////////
        //  Construction/destruction
    protected:
        NativeThreadRunner() = default;
        virtual ~NativeThreadRunner() = default;

        //////////
        //  Operations
    protected:
        /// \brief
        ///     Runs the native thread toi completion.
        /// \exception Exception
        ///     If an error occurs; the NativeThread
        ///     stops abnormally.
        virtual uint32_t    run() = 0;

        //////////
        //  Implementation
    private:
        NativeThread *  _nativeThread = nullptr;
    };

    /// \class NativeThread emuone-hades/API.hpp
    /// \brief The Thread runing native (C++) code.
    class EMUONE_HADES_PUBLIC NativeThread final
        :   public Thread
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeThread)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        NativeThread(
                Kernel * kernel, const Oid & oid, Identity * owner,
                NativeProcess * process,
                PriorityClass priorityClass,
                const QString name,
                NativeThreadRunner * runner
            );
        virtual ~NativeThread();

        //////////
        //  Implementation
    private:
        NativeThreadRunner *const   _runner;    //  owned by this NativeThread

        //////////
        //  Threads
    private:
        class _RunnerThread : public QThread
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_RunnerThread)

            //////////
            //  Construction/destruction
        public:
            explicit _RunnerThread(NativeThread * nativeThread)
                :   _nativeThread(nativeThread) { Q_ASSERT(_nativeThread != nullptr); }

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Implementation
        private:
            NativeThread *  _nativeThread;
        };
        _RunnerThread * _runnerThread = nullptr;
    };
}

//  End of emuone-hades/kernel/Thread.hpp
