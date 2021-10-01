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
    ComponentTypeList   getComponentTypes() const;
};

//  End of emuone-core/ComponentCategory.hpp
