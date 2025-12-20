//
//  emuone-core/Stereotype.cpp - VM Stereotype
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
    /// \class IStereotype emuone-core/API.hpp
    /// \brief The VM Type.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IStereotype
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IStereotype)

        //////////
        //  Construction/destruction
    protected:
        IStereotype() = default;
        virtual ~IStereotype() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QImage  smallImage() const = 0;
        virtual QImage  largeImage() const = 0;

        //////////
        //  Operations
    public:
    };

    /// \class StereotypeManager emuone-core/AI.hpp
    /// \brief The manager of known VM types.
    class EMUONE_CORE_PUBLIC StereotypeManager final
    {
        EMUONE_UTILITY_CLASS(StereotypeManager)

        /// \brief
        ///     Returns the set of all registered VM stereotypes.
        /// \return
        ///     Returns the set of all registered VM stereotypes.
        static auto     allStereotypes() -> Stereotypes;

        /// \brief
        ///     Registers the specified VM stereotype.
        /// \details
        ///     Registering an already-registered VM type
        ///     does nothing and returns true (success).
        /// \param stereotype
        ///     The VM stereotype to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerStereotype(IStereotype * stereotype);

        /// \brief
        ///     Un-registers the specified VM stereotype.
        /// \param stereotype
        ///     The VM stereotype to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterStereotype(IStereotype * stereotype);

        /// \brief
        ///     Finds a registered VM stereotype by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM stereotype with the
        ///     required mnemonic or nullptr if none found.
        static auto     findStereotype(const QString & mnemonic) -> IStereotype *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/Stereotype.cpp
