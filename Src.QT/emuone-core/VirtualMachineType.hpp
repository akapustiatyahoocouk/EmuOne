//
//  emuone-core/VirtualMachineType.cpp - VM VirtualMachineType
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
    /// \class IVirtualMachineType emuone-core/API.hpp
    /// \brief The VM Type.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IVirtualMachineType
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IVirtualMachineType)

        //////////
        //  Construction/destruction
    protected:
        IVirtualMachineType() = default;
        virtual ~IVirtualMachineType() = default;

        //////////
        //  Operations
    public:
    };

    /// \class VirtualMachineTypeManager emuone-core/AI.hpp
    /// \brief The manager of known VM types.
    class EMUONE_CORE_PUBLIC VirtualMachineTypeManager final
    {
        EMUONE_UTILITY_CLASS(VirtualMachineTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VM virtualMachineTypes.
        /// \return
        ///     Returns the set of all registered VM virtualMachineTypes.
        static auto     allVirtualMachineTypes() -> VirtualMachineTypes;

        /// \brief
        ///     Registers the specified VM virtualMachineType.
        /// \details
        ///     Registering an already-registered VM type
        ///     does nothing and returns true (success).
        /// \param virtualMachineType
        ///     The VM virtualMachineType to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerVirtualMachineType(IVirtualMachineType * virtualMachineType);

        /// \brief
        ///     Un-registers the specified VM virtualMachineType.
        /// \param virtualMachineType
        ///     The VM virtualMachineType to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterVirtualMachineType(IVirtualMachineType * virtualMachineType);

        /// \brief
        ///     Finds a registered VM virtualMachineType by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM virtualMachineType with the
        ///     required mnemonic or nullptr if none found.
        static auto     findVirtualMachineType(const QString & mnemonic) -> IVirtualMachineType *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };

    /// \class StandardVirtualMachineTypes emuone-core/API.hpp
    /// \brief Standard predefined VN virtualMachineTypes.
    class EMUONE_CORE_PUBLIC StandardVirtualMachineTypes final
    {
        EMUONE_UTILITY_CLASS(StandardVirtualMachineTypes)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all standard VM virtualMachineTypes.
        /// \return
        ///     The set of all standard VM virtualMachineTypes.
        static auto     all() -> VirtualMachineTypes;

        //////////
        //  VirtualMachineTypes
    public:
        /// \class VirtualMachine emuone-core/API.hpp
        /// \brief The "VM" virtualMachineType.
        class EMUONE_CORE_PUBLIC VirtualMachine final
            :   public virtual IVirtualMachineType
        {
            EMUONE_DECLARE_SINGLETON(VirtualMachine)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QImage  smallImage() const override;
            virtual QImage  largeImage() const override;
        };

        /// \class RemoteTerminal emuone-core/API.hpp
        /// \brief The "remote terminal" virtualMachineType.
        class EMUONE_CORE_PUBLIC RemoteTerminal final
            :   public virtual IVirtualMachineType
        {
            EMUONE_DECLARE_SINGLETON(RemoteTerminal)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QImage  smallImage() const override;
            virtual QImage  largeImage() const override;
        };
    };
}

//  End of emuone-core/VirtualMachineType.cpp
