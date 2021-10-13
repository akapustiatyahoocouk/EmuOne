//
//  emuone-core/ComponentCategory.hpp
//
//  The ComponentCategory ADT
//
//////////
#pragma once

namespace core
{
    //////////
    //  Represents a general "category" of components (e.g. "storage devices", "terminals", etc.
    //  Concrete derived classes will normally be singletons registered by plugins.
    class EMUONE_CORE_EXPORT ComponentCategory : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentCategory)

        //////////
        //  Construction/destruction
    public:
        ComponentCategory() {}
        virtual ~ComponentCategory() {}

        //////////
        //  Operations
    public:
        //  Returns an unordered list of all known component types that belong
        //  to this component category
        ComponentTypeList   componentTypes() const;

        //////////
        //  Registry
    public:
        static ComponentCategoryList    all();
        static ComponentCategory *      findByMnemonic(const QString & mnemonic);
        static bool                     registerComponentCategory(ComponentCategory * componentCategory);
    private:
        static ComponentCategoryList    _registry;
    };

    //////////
    //  The "standard" (predefined) component categories
    class EMUONE_CORE_EXPORT StandardComponentCategories
    {
        UTILITY_CLASS(StandardComponentCategories)

        //////////
        //  Types
    public:
        class EMUONE_CORE_EXPORT Memory : public ComponentCategory
        {
            DECLARE_SINGLETON(Memory)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon;
            mutable QIcon       _largeIcon;
        };

        class EMUONE_CORE_EXPORT Processors : public ComponentCategory
        {
            DECLARE_SINGLETON(Processors)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon;
            mutable QIcon       _largeIcon;
        };

        class EMUONE_CORE_EXPORT Terminals : public ComponentCategory
        {
            DECLARE_SINGLETON(Terminals)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon;
            mutable QIcon       _largeIcon;
        };

        class EMUONE_CORE_EXPORT Software : public ComponentCategory
        {
            DECLARE_SINGLETON(Software)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon;
            mutable QIcon       _largeIcon;
        };
    };
}

//  End of emuone-core/ComponentCategory.hpp
