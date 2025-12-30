//
//  emuone-core/ComponentType.hpp - VM component type
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
    /// \brief The VM component Category.
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
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered component types that
        ///     belong to this component category.
        /// \return
        ///     The set of all registered component types that
        ///     belong to this component category.
        auto            componentTypes() const -> ComponentTypes;
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
        ///     Returns the set of all registered VM Component Categories.
        /// \return
        ///     Returns the set of all registered VM Component Categories.
        static auto     all() -> ComponentCategories;

        /// \brief
        ///     Registers the specified Component Category.
        /// \details
        ///     Registering an already-registered Component Category
        ///     does nothing and returns true (success).
        /// \param componentCategory
        ///     The VM Component Category to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(IComponentCategory * componentCategory);

        /// \brief
        ///     Un-registers the specified VM Component Category.
        /// \param componentCategory
        ///     The VM Component Category to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IComponentCategory * componentCategory);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered VM Component Category with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> IComponentCategory *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };

    /// \class StandardComponentCategories emuone-core/API.hpp
    /// \brief Standard predefined component categories.
    class EMUONE_CORE_PUBLIC StandardComponentCategories final
    {
        EMUONE_UTILITY_CLASS(StandardComponentCategories)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all standard component categories.
        /// \return
        ///     The set of all standard component categories.
        static auto     all() -> ComponentCategories;

        //////////
        //  Component categories
    public:
        class EMUONE_CORE_PUBLIC Processors final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(Processors)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC Memory final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(Memory)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC Storage final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(Storage)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC OnboardDevices final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(OnboardDevices)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC UiDevices final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(UiDevices)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC Software final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(Software)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        class EMUONE_CORE_PUBLIC Miscellaneous final
            :   public virtual IComponentCategory
        {
            EMUONE_DECLARE_SINGLETON(Miscellaneous)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };
    };
}

//  End of emuone-core/ComponentCategory.hpp
