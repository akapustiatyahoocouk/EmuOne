//
//  emuone-core/ApplianceType.cpp - VA Type
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
    /// \class IApplianceType emuone-core/API.hpp
    /// \brief The VA Type.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IApplianceType
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IApplianceType)

        //////////
        //  Construction/destruction
    protected:
        IApplianceType() = default;
        virtual ~IApplianceType() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QImage  smallImage() const = 0;
        virtual QImage  largeImage() const = 0;

        //////////
        //  Operations
    public:
    };

    /// \class ApplianceTypeManager emuone-core/AI.hpp
    /// \brief The manager of known VA types.
    class EMUONE_CORE_PUBLIC ApplianceTypeManager final
    {
        EMUONE_UTILITY_CLASS(ApplianceTypeManager)

        /// \brief
        ///     Returns the set of all registered VA Types.
        /// \return
        ///     Returns the set of all registered VA Types.
        static auto     allApplianceTypes() -> ApplianceTypes;

        /// \brief
        ///     Registers the specified VA type.
        /// \details
        ///     Registering an already-registered VA type
        ///     does nothing and returns true (success).
        /// \param type
        ///     The VA Type to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerApplianceType(IApplianceType * vaType);

        /// \brief
        ///     Un-registers the specified VA Type.
        /// \param type
        ///     The VA Type to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterApplianceType(IApplianceType * vaType);

        /// \brief
        ///     Finds a registered VA type by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VA Type with the
        ///     required mnemonic or nullptr if none found.
        static auto     findApplianceType(const QString & mnemonic) -> IApplianceType *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/ApplianceType.cpp
