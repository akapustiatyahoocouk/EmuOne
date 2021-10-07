//
//  emuone-core/ComponentType.hpp
//
//  The ComponentType ADT
//
//////////
#pragma once

namespace core
{
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
        //  StockObject
    public:
        virtual QIcon       getSmallIcon() const override { return getCategory()->getSmallIcon(); }
        virtual QIcon       getLargeIcon() const override { return getCategory()->getLargeIcon(); }

        //////////
        //  Operations
    public:
        //  Returns the category to which this component type belongs.
        virtual ComponentCategory * getCategory() const = 0;

        //  Checks if components of this type are "compatible with" (i.e. can be
        //  used directly by) virtual appliances of the specified "architecture"
        virtual bool                isCompatibleWith(Architecture * architecture) const = 0;

        //  Creates and returns a new Component of this type with default configuration
        virtual Component *         createComponent() = 0;

        //////////
        //  Registry
    public:
        static ComponentTypeList    getAll();
        static ComponentType *      findByMnemonic(const QString & mnemonic);
        static bool                 registerComponentType(ComponentType * componentType);
    private:
        static ComponentTypeList    _registry;
    };
}

//  End of emuone-core/ComponentType.hpp
