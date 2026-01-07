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
#pragma once
#include "emuone-hades/API.hpp"

namespace emuone::hades::kernel
{
    /// \class NativeThreadRunner emuone-hades/API.hpp
    /// \brief An agent that implements a NativeThread.
    class EMUONE_HADES_PUBLIC NativeThreadRunner
        :   public QObject
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NativeThreadRunner)

        friend class Kernel;
        friend class NativeThread;
        friend class SystemCalls;
        friend class systemprocesses::Init;
        friend class systemprocesses::DeviceManager;

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
        //  Signals
    signals:
        /// \brief
        ///     Emitted when new output is sent to the
        ///     Kernel Console.
        /// \param output
        ///     The new output sent to the Kernel Console.
        void            kernelConsoleOutput(QString output);

        //////////
        //  Implementation
    private:
        NativeThread *  _nativeThread = nullptr;
    };
}

//  End of emuone-hades/kernel/NativeThreadRunner.hpp
