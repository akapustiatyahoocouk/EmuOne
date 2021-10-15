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
    //  A SCP/360 application that runs c++ code, also acts as a process factory
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
        //  Operations
    public:
        virtual EmulatedProcess *   createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent) = 0;

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
