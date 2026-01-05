//
//  emuone-hades/kernel/NativeThreadRunner.hpp - The HADES OS kernel NativeThreadRunner
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
    /// \class SystemCalls emuone-hades/API.hpp
    /// \brief The system calls PI for a NativeThread.
    class EMUONE_HADES_PUBLIC SystemCalls final
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemCalls)

        friend class NativeThreadRunner;

        //////////
        //  Construction/destruction
    private:
        explicit SystemCalls(NativeThreadRunner * runner)
            :   _runner(runner) { Q_ASSERT(_runner != nullptr); }
        ~SystemCalls() = default;

        //////////
        //  System calls
        //  TODO organize into groups
    public:
        void            signal(int sig, kernel::SignalDisposition handler);
        void            signal(int sig, kernel::NativeSignalHandler handler);
        void            yield();

        //////////
        //  Implementation
    private:
        NativeThreadRunner *    _runner;

        //  Helpers
        void            _processPendngSignals();
    };

    /// \class NativeThreadRunner emuone-hades/API.hpp
    /// \brief An agent that implements a NativeThread.
    class EMUONE_HADES_PUBLIC NativeThreadRunner
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeThreadRunner)

        friend class Kernel;
        friend class NativeThread;
        friend class SystemCalls;

        //////////
        //  Construction/destruction
    protected:
        NativeThreadRunner() = default;
        virtual ~NativeThreadRunner() = default;

        //////////
        //  Properties
    public:
        /// \brief
        ///     The system calls API for the Runner.
        SystemCalls     systemCalls {this };

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
}

//  End of emuone-hades/kernel/NativeThreadRunner.hpp
