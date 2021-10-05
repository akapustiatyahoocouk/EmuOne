//
//  emuone-ibm360/Storage.hpp
//
//  IBM/360 storage
//
//////////
#pragma once

//////////
//  The IBM/360 storage
class EMUONE_IBM360_EXPORT Ibm360Storage : public Component
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm360Storage)

    //////////
    //  Types
public:
    //  The "type" of IBM/360 mstorage
    class EMUONE_IBM360_EXPORT Type : public ComponentType
    {
        DECLARE_SINGLETON(Type)

        //////////
        //  StockObject
    public:
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;

        //////////
        //  ComponentType
    public:
        virtual ComponentCategory * getCategory() const override;
        virtual bool        isCompatibleWith(Architecture * architecture) const override;
    };

    //////////
    //  Construction/destruction
public:
    Ibm360Storage(uint32_t size);
    virtual ~Ibm360Storage();

    //////////
    //  Component
public:
    virtual ComponentType * getType() const override;
};

//  End of emuone-ibm360/Storage.hpp
