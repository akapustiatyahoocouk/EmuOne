//
//  emuone-core/ComponentAdaptorType.hpp - VM component adaptor type
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
    /// \class IComponentAdaptorType emuone-core/API.hpp
    /// \brief The VM component adaptor type.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IComponentAdaptorType
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponentAdaptorType)

        //////////
        //  Construction/destruction
    protected:
        IComponentAdaptorType() = default;
        virtual ~IComponentAdaptorType() = default;

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
        ///     Returns the architecture to which adaptors of this
        ///     type adapt otherwise incompatible components.
        /// \return
        ///     The architecture to which adaptors of this
        ///     type adapt otherwise incompatible components.
        virtual auto    architecture() const -> IArchitecture * = 0;

        /// \brief
        ///     Returns the type of components which ara adapted to
        ///     otherwise incompatible architectures by adaptors of this type.
        /// \return
        ///     The type of components which ara adapted to otherwise
        ///     incompatible architectures by adaptors of this type.
        virtual auto    componentType() const -> IComponentType * = 0;

        /// \brief
        ///     Checks whether ComponentAdaptors of this type can
        ///     save their runtime state and reload it later on.
        /// \return
        ///     True if ComponentAdaptors of this type can
        ///     save their runtime state and reload it later on,
        ///     false if not.
        virtual bool    isPersistable() const = 0;

        /// \brief
        ///     Creates the component adaptor for the specified owner/component.
        /// \param virtualMachine
        ///     The VM for which the adaptor is created.
        /// \param component
        ///     The component for which the adaptor is created.
        /// \return
        ///     The newly created ComponentAdaptor.
        virtual auto    createAdaptor(
                                VirtualMachine * virtualMachine,
                                IComponent * component
                            ) -> IComponentAdaptor * = 0;
    };

    /// \class ComponentAdaptorTypeManager emuone-core/API.hpp
    /// \brief The manager of known VAComponent Types.
    class EMUONE_CORE_PUBLIC ComponentAdaptorTypeManager final
    {
        EMUONE_UTILITY_CLASS(ComponentAdaptorTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VM Component Adaptor Types.
        /// \return
        ///     Returns the set of all registered VM Component Adaptor Types.
        static auto     all() -> ComponentAdaptorTypes;

        /// \brief
        ///     Registers the specified Component Type.
        /// \details
        ///     Registering an already-registered Component Adaptor Type
        ///     does nothing and returns true (success).
        /// \param componentAdaptorType
        ///     The VM Component Adaptor Type to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(IComponentAdaptorType * componentAdaptorType);

        /// \brief
        ///     Un-registers the specified VM Component Adaptor Type.
        /// \param componentAdaptorType
        ///     The VM Component Adaptor Type to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IComponentAdaptorType * componentAdaptorType);

        /// \brief
        ///     Finds a registered component adaptor type mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM Component Type with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> IComponentAdaptorType *;

        /// \brief
        ///     Finds the component adaptor type for the specified
        ///     architecture/component type combination.
        /// \param architecture
        ///     The architecture to adapt components to.
        /// \param componentType
        ///     The component type tp adapt.
        /// \return
        ///     If there exists a component adaptor type that adapts
        ///     components of the specified type to the specified
        ///     architecture, returns it; otherwise returns nullptr.
        ///     If several choices are available, returns an arbitrarily
        ///     selected one of them.
        static auto     find(IArchitecture * architecture,
                             IComponentType * componentType
                            ) -> IComponentAdaptorType *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/ComponentAdaptorType.hpp
