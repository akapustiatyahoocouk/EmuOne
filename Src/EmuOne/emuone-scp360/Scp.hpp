//
//  emuone-scp360/Scp.hpp
//
//  SCP/360 Monitor
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A SCP/360 "Monitor"
    class EMUONE_SCP360_EXPORT Scp : public ibm360::Monitor
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Scp)

        //////////
        //  Types
    public:
        //  The "type" of SCP/360 monitor
        class EMUONE_IBM360_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;

            //////////
            //  ComponentType
        public:
            virtual core::ComponentCategory *   category() const override;
            virtual bool        isCompatibleWith(core::Architecture * architecture) const override;
            virtual Scp *       createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        explicit Scp(const QString & name);
        virtual ~Scp();

        //////////
        //  Component
    public:
        virtual Type *      type() const override;
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString     shortStatus() const override;
        virtual core::ComponentUi * createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State       state() const override;
        virtual void        connect() override;     //  throws VirtualApplianceException
        virtual void        initialise() override;  //  throws VirtualApplianceException;
        virtual void        start() override;       //  throws VirtualApplianceException
        virtual void        stop() noexcept override;
        virtual void        deinitialise() noexcept override;
        virtual void        disconnect() noexcept override;

        //////////
        //  Component (serialisation)
    public:
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        QMap<uint16_t, Device*>         _devices;   //  ...keyed by I/O address
        QMap<uint16_t, DeviceDriver*>   _deviceDrivers;   //  ...keyed by I/O address
        ProcessList         _processes; //  ...all that exist

        //  Helpers
        void                _registerDevice(Device * device);   //  throws VirtualApplianceException on error
        void                _createDeviceDrivers();
        void                _destroyDeviceDrivers();

        //////////
        //  Threads
    private:
        class _WorkerThread : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Construction/destruction
        public:
            _WorkerThread(Scp *const scp);
            virtual ~_WorkerThread();

            //////////
            //  QThread
        public:
            virtual void    run() override;

            //////////
            //  Operations
        public:
            void            requestStop() { _stopRequested = true;  }

            //////////
            //  Implementation
        private:
            Scp *const      _scp;
            volatile bool   _stopRequested = false;
        };
        _WorkerThread *     _workerThread = nullptr;
    };
}

//  End of emuone-scp360/Scp.hpp
