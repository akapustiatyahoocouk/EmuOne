//
//  emuone-ibm360/Architecture.hpp
//
//  IBM/360 architecture
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  IBM/360 architecture
    class EMUONE_IBM360_EXPORT Architecture : public core::Architecture
    {
        DECLARE_SINGLETON(Architecture)

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
        mutable QIcon       _smallIcon = QIcon();
        mutable QIcon       _largeIcon = QIcon();
    };
}

//  End of emuone-ibm360/Architecture.hpp
