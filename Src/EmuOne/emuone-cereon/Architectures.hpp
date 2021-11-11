//
//  emuone-cereon/Architectures.hpp
//
//  Cereon-based architectures
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  A generic Cereon-based architecture
    class EMUONE_CEREON_EXPORT CereonArchitecture : public core::Architecture
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CereonArchitecture)

        //////////
        //  Construction/destruction - from derived classes only
    protected:
        CereonArchitecture() {}
        virtual ~CereonArchitecture() {}

        //////////
        //  StockObject
    public:
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;

        //////////
        //  Implementation
    private:
        mutable QIcon       _smallIcon = QIcon();
        mutable QIcon       _largeIcon = QIcon();
    };

    //////////
    //  Cereon Workstation architecture
    class EMUONE_CEREON_EXPORT CereonWorkstationArchitecture : public CereonArchitecture
    {
        DECLARE_SINGLETON(CereonWorkstationArchitecture)

        //////////
        //  StockObject
    public:
        virtual QString     mnemonic() const override;
        virtual QString     displayName() const override;
    };
}

//  End of emuone-cereon/Architecture.hpp
