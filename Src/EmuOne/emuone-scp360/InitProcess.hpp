//
//  emuone-scp360/InitProcess.hpp
//
//  A SCP/360 "INIT" Process
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A SCP/360 INIT Process
    class EMUONE_SCP360_EXPORT InitProcess : public EmulatedProcess
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(InitProcess)

        //////////
        //  Types
    public:
        //////////
        //  A SCP/360 INIT Application
        class EMUONE_SCP360_EXPORT InitApplication : public EmulatedApplication
        {
            DECLARE_SINGLETON(InitApplication)

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
        InitProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent);
        virtual ~InitProcess();

        //////////
        //  EmulatedProcess
    public:
        virtual EmulatedApplication *   application() const override;
    protected:
        virtual uint16_t        run() override;
    };
}

//  End of emuone-scp360/InitProcess.hpp
