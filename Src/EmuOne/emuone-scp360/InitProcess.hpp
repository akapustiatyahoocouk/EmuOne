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
        //  A SCP/360 INIT Application
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
        InitProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent);
        virtual ~InitProcess();

        //////////
        //  EmulatedProcess
    public:
        virtual EmulatedApplication *   application() const override;
    protected:
        virtual ErrorCode       run() override;

        //////////
        //  Implementation
    private:
        class _InitialiseDeviceCompletionListener : public DeviceDriver::IoCompletionListener
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_InitialiseDeviceCompletionListener)

            //////////
            //  Construction/destruction
        public:
            _InitialiseDeviceCompletionListener(Device * device);
            virtual ~_InitialiseDeviceCompletionListener();

            //////////
            //  DeviceDriver::IoCompletionListener
        public:
            virtual void        ioCompleted(Device * device, ErrorCode errorCode) override;

            //////////
            //  Properties
        public:
            Device *            device;
            ErrorCode           errorCode = ErrorCode::ERR_UNK;
            bool                completed = false;
        };
        QList<_InitialiseDeviceCompletionListener*> _initialiseDeviceCompletionListeners;

        //  Helpers
        void                    _initialiseDevices();
        void                    _createLoginProcesses();
    };
}

//  End of emuone-scp360/InitProcess.hpp
