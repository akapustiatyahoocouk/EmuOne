//
//  emuone-util/StockObject.hpp
//
//  Stock objects
//
//////////

//////////
//  A generic object coming from a fixed "stock"
class EMUONE_UTIL_EXPORT StockObject
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StockObject)

    //////////
    //  Construction/destruction
public:
    StockObject() {}
    virtual ~StockObject() {}

    //////////
    //  Operations
public:
    virtual QString     mnemonic() const = 0;
    virtual QString     displayName() const = 0;
    virtual QIcon       smallIcon() const = 0;
    virtual QIcon       largeIcon() const = 0;
};

//  End of emuone-util/StockObject.hpp
