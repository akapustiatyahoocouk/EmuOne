//
//  emuone-core/ComponentType.cpp - VA component type
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
    /// \class IComponentCategory emuone-core/API.hpp
    /// \brief The VA component Category.
    /// \details Concrete subclasses will be singletons registered by Components.
    class EMUONE_CORE_PUBLIC IComponentCategory
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponentCategory)

        //////////
        //  Construction/destruction
    protected:
        IComponentCategory() = default;
        virtual ~IComponentCategory() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QImage  smallImage() const = 0;
        virtual QImage  largeImage() const = 0;

        //////////
        //  Operations
    public:
    };

    /// \class ComponentCategoryManager emuone-core/API.hpp
    /// \brief The manager of known VAComponent Categories.
    class EMUONE_CORE_PUBLIC ComponentCategoryManager final
    {
        EMUONE_UTILITY_CLASS(ComponentCategoryManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered VA Component Categories.
        /// \return
        ///     Returns the set of all registered VA Component Categories.
        static auto     allComponentCategories() -> ComponentCategories;

        /// \brief
        ///     Registers the specified Component Category.
        /// \details
        ///     Registering an already-registered Component Category
        ///     does nothing and returns true (success).
        /// \param componentCategory
        ///     The VA Component Category to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerComponentCategory(IComponentCategory * componentCategory);

        /// \brief
        ///     Un-registers the specified VA Component Category.
        /// \param componentCategory
        ///     The VA Component Category to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterComponentCategory(IComponentCategory * componentCategory);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VA Component Category with the
        ///     required mnemonic or nullptr if none found.
        static auto     findComponentCategory(const QString & mnemonic) -> IComponentCategory *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-core/ComponentCategory.cpp
