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
    virtual QString     getMnemonic() const = 0;
    virtual QString     getDisplayName() const = 0;
    virtual QIcon       getSmallIcon() const = 0;
    virtual QIcon       getLargeIcon() const = 0;
};

//  End of emuone-util/StockObject.hpp
