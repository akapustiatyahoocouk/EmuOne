//
//  emuone-core/Template.cpp - VA template
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
    /// \class ITemplate emuone-core/API.hpp
    /// \brief The VA Template.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC ITemplate
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ITemplate)

        //////////
        //  Construction/destruction
    protected:
        ITemplate() = default;
        virtual ~ITemplate() = default;

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
        ///     creates Virtual Appliances.
        /// \return
        ///     Returns the Architecture for which this Template
        ///     creates Virtual Appliances.
        virtual auto    architecture() const -> IArchitecture * = 0;
    };

    /// \class TemplateManager emuone-core/API.hpp
    /// \brief The manager of known VA Templates.
    class EMUONE_CORE_PUBLIC TemplateManager final
    {
        EMUONE_UTILITY_CLASS(TemplateManager);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VA templates.
        /// \return
        ///     The set of all registered VA templates.
        static auto     allTemplates() -> Templates;

        /// \brief
        ///     Registers the specified VA template.
        /// \details
        ///     Registering an already-registered VA template
        ///     does nothing and returns true (success).
        /// \param vaTemplate
        ///     The VA template to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerTemplate(ITemplate * vaTemplate);

        /// \brief
        ///     Un-registers the specified VA template.
        /// \param vaTemplate
        ///     The VA template to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterTemplate(ITemplate * vaTemplate);

        /// \brief
        ///     Finds a registered VA template by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VA template with the
        ///     required mnemonic or nullptr if none found.
        static auto     findTemplate(const QString & mnemonic) -> ITemplate *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/Template.cpp
