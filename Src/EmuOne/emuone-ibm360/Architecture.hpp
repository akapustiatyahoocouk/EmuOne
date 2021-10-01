//
//  emuone-ibm360/Architecture.hpp
//
//  IBM/360 architecture
//
//////////
#pragma once

class EMUONE_IBM360_EXPORT Ibm360Architecture : public Architecture
{
    DECLARE_SINGLETON(Ibm360Architecture)

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
    mutable QIcon       _smallIcon;
    mutable QIcon       _largeIcon;
};

//  End of emuone-ibm360/Architecture.hpp
