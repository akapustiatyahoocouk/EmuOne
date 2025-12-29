//
//  emuone-core/ComponentType.cpp - VM component type
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
    /// \class IComponentType emuone-core/API.hpp
    /// \brief The VM component type.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IComponentType
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponentType)

        //////////
        //  Construction/destruction
    protected:
        IComponentType() = default;
        virtual ~IComponentType() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QIcon   smallIcon() const;
        virtual QIcon   largeIcon() const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the ComponentCategory to which this ComponentType belongs.
        /// \return
        ///     The ComponentCategory to which this ComponentType belongs.
        virtual auto    category() const -> IComponentCategory * = 0;

        /// \brief
        ///     Checks whether components of this type are compatible with
        ///     (i.e. can work in) virtual machines of the specified Architecture.
        /// \param architecture
        ///     The VM architecture to check for compatibility with.
        /// \return
        ///     True if components of this type are compatible with (i.e. can work
        ///     in) virtual machines of the specified Architecture, false if not.
        virtual bool    isCompatibleWith(IArchitecture * architecture) const = 0;

        /// \brief
        ///     Checks whether components of this type are compatible with
        ///     (i.e. can work in) virtual machines of the specified type.
        /// \param virtualMachineType
        ///     The VM type to check for compatibility with.
        /// \return
        ///     True if components of this type are compatible with (i.e. can work
        ///     in) virtual machines of the specified type, false if not.
        virtual bool    isCompatibleWith(IVirtualMachineType * virtualMachineType) const = 0;

        /// \brief
        ///     Checks whether Components of this type can
        ///     save their runtime state and reload it later on.
        /// \return
        ///     True if Components of this type can
        ///     save their runtime state and reload it later on,
        ///     false if not.
        virtual bool    isPersistable() const = 0;

        /// \brief
        ///     Creates a new unbound Component of this type.
        /// \details
        ///     The Component is created with default configuration.
        ///     It is the caller's responsibility to either destroy the
        ///     Component or to pass ownership of the Component to a VM.
        /// \return
        ///     The newly created Component.
        virtual auto    createComponent() -> IComponent * = 0;
    };

    /// \class ComponentTypeManager emuone-core/API.hpp
    /// \brief The manager of known VAComponent Types.
    class EMUONE_CORE_PUBLIC ComponentTypeManager final
    {
        EMUONE_UTILITY_CLASS(ComponentTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VM Component Types.
        /// \return
        ///     Returns the set of all registered VM Component Types.
        static auto     allComponentTypes() -> ComponentTypes;

        /// \brief
        ///     Registers the specified Component Type.
        /// \details
        ///     Registering an already-registered Component Type
        ///     does nothing and returns true (success).
        /// \param componentType
        ///     The VM Component Type to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerComponentType(IComponentType * componentType);

        /// \brief
        ///     Un-registers the specified VM Component Type.
        /// \param componentType
        ///     The VM Component Type to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterComponentType(IComponentType * componentType);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM Component Type with the
        ///     required mnemonic or nullptr if none found.
        static auto     findComponentType(const QString & mnemonic) -> IComponentType *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/ComponentType.cpp
