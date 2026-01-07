//
//  emuone-hades/kernel/SystemCalls.hpp - System calls API for native threads
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
        //  Types
    public:
        struct Atom { uint32_t oid; };  //  Kernel's Oid

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
        KErrno          kwrite(const QString & s);

        KErrno          signal(int sig, kernel::SignalDisposition handler);
        KErrno          signal(int sig, kernel::NativeSignalHandler handler);

        KErrno          getAtom(const QString & value, Atom & atom);
        KErrno          releaseAtom(Atom atom);
        KErrno          yield();

        //////////
        //  Implementation
    private:
        NativeThreadRunner *    _runner;
        QString         _kernelConsoleBuffer;

        //  Helpers
        void            _processPendngSignals();
    };
}

//  End of emuone-hades/kernel/SystemCalls.hpp
