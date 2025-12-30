//
//  emuone-core/Architecture.hpp - VM Architecture
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

namespace emuone::core
{
    /// \class IArchitecture emuone-core/API.hpp
    /// \brief The VM architecture.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IArchitecture
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IArchitecture)

        //////////
        //  Construction/destruction
    protected:
        IArchitecture() = default;
        virtual ~IArchitecture() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Performs the quick validity check of the specified VM.
        /// \details
        ///     The "quick check" means checking that all components mandatory
        ///     for this Architecture are actually present in the VM and in the
        ///     proper numbers (e.g. at most one Kernel/Monitor, etc.)
        /// \return
        ///     True if the VM is valid fr this architecture, false if not.
        virtual bool    isValid(VirtualMachine * virtualMachine) const = 0;
    };

    /// \class ArchitectureManager emuone-core/API.hpp
    /// \brief The manager of known VM architectures.
    class EMUONE_CORE_PUBLIC ArchitectureManager final
    {
        EMUONE_UTILITY_CLASS(ArchitectureManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VM architectures.
        /// \return
        ///     Returns the set of all registered VM architectures.
        static auto     all() -> Architectures;

        /// \brief
        ///     Registers the specified Architecture.
        /// \details
        ///     Registering an already-registered Architecture
        ///     does nothing and returns true (success).
        /// \param architecture
        ///     The VM architecture to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(IArchitecture * architecture);

        /// \brief
        ///     Un-registers the specified VM architecture.
        /// \param architecture
        ///     The VM architecture to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IArchitecture * architecture);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM architecture with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> IArchitecture *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/Architecture.hpp
