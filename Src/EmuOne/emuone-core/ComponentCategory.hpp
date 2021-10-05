//
//  emuone-core/ComponentCategory.hpp
//
//  The ComponentCategory ADT
//
//////////
#pragma once

//////////
//  Represents a general "category" of components (e.g. "storage devices", "terminals", etc.
//  Concrete derived classes will normally be singletons registered by plugins.
class EMUONE_CORE_EXPORT ComponentCategory : public StockObject
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
    ComponentTypeList   getComponentTypes() const;
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
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;
        virtual QIcon       getSmallIcon() const override;
        virtual QIcon       getLargeIcon() const override;

        //////////
        //  Implementation
    private:
        mutable QIcon      _smallIcon;
        mutable QIcon       _largeIcon;
    };
};

//  End of emuone-core/ComponentCategory.hpp
