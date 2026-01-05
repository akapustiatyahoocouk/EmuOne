//
//  emuone-hades/SystemProcess.hpp - A process emulated in C++
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

namespace emuone::hades
{
    /// \class ISystemProcess emuone-hades/API.hpp
    /// \brief A process emulated in C++.
    class EMUONE_HADES_PUBLIC ISystemProcess
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ISystemProcess)

        //////////
        //  Types
    public:
        using Runner = kernel::NativeThreadRunner;

        //////////
        //  This is an interface
    protected:
        ISystemProcess() = default;
        virtual ~ISystemProcess() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QString mnemonic() const override;
        virtual QString displayName() const override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the full virtual "path" of an imaginary
        ///     executable file which, when "started", implements
        ///     this system process *e.g. "SYSTEM:/bin/init").
        /// \return
        ///     The full virtual "path" of an imaginary
        ///     executable file which, when "started", implements
        ///     this system process.
        virtual QString virtualPath() const = 0;

        /// \brief
        ///     Returns the name of this SystemProcess
        ///     (e.g. "init").
        /// \return
        ///     The name of this SystemProcess..
        virtual QString processName() const = 0;

        /// \brief
        ///     Created a new Runner for a native thread
        ///     of this SystemProcess.
        virtual Runner *createRunner() = 0;
    };

    /// \class SystemProcessManager emuone-hades/API.hpp
    /// \brief The manager of known HADES SystemProcesses.
    class EMUONE_HADES_PUBLIC SystemProcessManager final
    {
        EMUONE_UTILITY_CLASS(SystemProcessManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered SystemProcesses.
        /// \return
        ///     Returns the set of all registered SystemProcesses.
        static auto     all() -> SystemProcesses;

        /// \brief
        ///     Registers the specified SystemProcess.
        /// \details
        ///     Registering an already-registered SystemProcess
        ///     does nothing and returns true (success).
        /// \param systemProcess
        ///     The SystemProcess to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(ISystemProcess * systemProcess);

        /// \brief
        ///     Un-registers the specified SystemProcess.
        /// \param systemProcess
        ///     The SystemProcess to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(ISystemProcess * systemProcess);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered SystemProcess with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> ISystemProcess *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-hades/SystemProcess.hpp
