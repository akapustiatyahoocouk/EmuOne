//
//  emuone-scp360/LoginProcess.hpp
//
//  A SCP/360 "LOGIN" Process
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A SCP/360 LOGIN Process
    class EMUONE_SCP360_EXPORT LoginProcess : public EmulatedProcess
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(LoginProcess)

        //////////
        //  A SCP/360 LOGIN Application
    public:
        class EMUONE_SCP360_EXPORT Application : public EmulatedApplication
        {
            DECLARE_SINGLETON(Application)

            //////////
            //  util::StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;

            //////////
            //  EmulatedApplication
        public:
            virtual EmulatedProcess *   createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent) override;
        };

        //////////
        //  Construction/destruction - from friends only
    private:
        LoginProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent);
        virtual ~LoginProcess();

        //////////
        //  EmulatedProcess
    public:
        virtual EmulatedApplication *   application() const override;
    protected:
        virtual ErrorCode       run() override;
    };
}

//  End of emuone-scp360/LoginProcess.hpp
