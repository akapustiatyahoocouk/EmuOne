//
//  emuone-core/ComponentType.hpp
//
//  The ComponentType ADT
//
//////////
#pragma once

//////////
//  Represents a type of a component; also acts as a factory for components of that type.
class EMUONE_CORE_EXPORT ComponentType : public StockObject
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentType)

    //////////
    //  Construction/destruction
public:
    ComponentType() {}
    virtual ~ComponentType() {}

    //////////
    //  Operations
public:
    //  Returns the category to which this component type belongs.
    virtual ComponentCategory * getCategory() const = 0;
};

//  End of emuone-core/ComponentType.hpp
