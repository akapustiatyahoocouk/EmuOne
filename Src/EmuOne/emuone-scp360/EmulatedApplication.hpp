//
//  emuone-scp360/EmulatedApplication.hpp
//
//  A SCP/360 application that runs c++ code
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A SCP/360 application that runs c++ code
    class EMUONE_SCP360_EXPORT EmulatedApplication : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EmulatedApplication)

        //////////
        //  Construction/destruction
    public:
        EmulatedApplication() {}
        virtual ~EmulatedApplication() {}

        //////////
        //  StockObject
    public:
        virtual QIcon       smallIcon() const override { return core::StandardComponentCategories::Software::getInstance()->smallIcon(); }
        virtual QIcon       largeIcon() const override { return core::StandardComponentCategories::Software::getInstance()->largeIcon(); }

        //////////
        //  Registry
    public:
        static EmulatedApplicationList  all();
        static EmulatedApplication *    findByMnemonic(const QString & mnemonic);
        static bool                     registerEmulatedApplication(EmulatedApplication * cmulatedApplication);
    private:
        static EmulatedApplicationList  _registry;
    };
}

//  End of emuone-scp360/EmulatedApplication.hpp
