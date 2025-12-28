//
//  emuone-core/VirtualMachineTemplate.cpp - VM template
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
    /// \class IVirtualMachineTemplate emuone-core/API.hpp
    /// \brief The VM Template.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IVirtualMachineTemplate
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IVirtualMachineTemplate)

        //////////
        //  Construction/destruction
    protected:
        IVirtualMachineTemplate() = default;
        virtual ~IVirtualMachineTemplate() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QImage  smallImage() const;
        virtual QImage  largeImage() const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Architecture for which this Template
        ///     creates virtual machines.
        /// \return
        ///     Returns the Architecture for which this Template
        ///     creates virtual machiness.
        virtual auto    architecture() const -> IArchitecture * = 0;

        /// \brief
        ///     Returns the VirtualMachineType for which this Template
        ///     creates virtual machiness.
        /// \return
        ///     Returns the VirtualMachineType for which this Template
        ///     creates virtual machiness.
        virtual auto    virtualMachineType() const -> IVirtualMachineType * = 0;
    };

    /// \class TemplateManager emuone-core/API.hpp
    /// \brief The manager of known VM Templates.
    class EMUONE_CORE_PUBLIC VirtualMachineTemplateManager final
    {
        EMUONE_UTILITY_CLASS(VirtualMachineTemplateManager);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VM templates.
        /// \return
        ///     The set of all registered VM templates.
        static auto     all() -> VirtualMachineTemplates;

        /// \brief
        ///     Registers the specified VM template.
        /// \details
        ///     Registering an already-registered VM template
        ///     does nothing and returns true (success).
        /// \param virtualMachineTemplate
        ///     The VM template to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(IVirtualMachineTemplate * virtualMachineTemplate);

        /// \brief
        ///     Un-registers the specified VM template.
        /// \param virtualMachineTemplate
        ///     The VM template to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IVirtualMachineTemplate * virtualMachineTemplate);

        /// \brief
        ///     Finds a registered VM template by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM template with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> IVirtualMachineTemplate *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/VirtualMachineTemplate.cpp
