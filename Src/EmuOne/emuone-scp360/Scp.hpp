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

        friend class Object;
        friend class ::ScpEditor;
        friend class EmulatedProcess;
        friend class InitProcess;

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

        //  A "shared folder" configured for an SCP monitor, which makes a host
        //  OS directory behave like a SCP "volume"
        class EMUONE_SCP360_EXPORT SharedFolder final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SharedFolder)

            friend class Scp;

            //////////
            //  Construction/destruction - from friends only
        private:
            SharedFolder(const QString & volumeName, const QString & hostPath)
                :   _volumeName(volumeName), _hostPath(QFileInfo(hostPath).canonicalFilePath())
            {
                Q_ASSERT(Validator::isValidVolumeName(_volumeName));
            }
            ~SharedFolder() {}

            //////////
            //  Operations
        public:
            QString             volumeName() const { return _volumeName; }
            QString             hostPath() const { return _hostPath; }

            //////////
            //  Implementation
        private:
            QString             _volumeName;
            QString             _hostPath;
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
        //  ibm360::Monitor - interrupt handling.
    public:
        virtual void        onIoInterruption(uint16_t interruptionCode) override;
        virtual void        onProgramInterruption(uint16_t interruptionCode) override;
        virtual void        onSvcInterruption(uint16_t interruptionCode) override;
        virtual void        onExternalInterruption(uint16_t interruptionCode) override;
        virtual void        onMachineCheckInterruption(uint16_t interruptionCode) override;

        //////////
        //  Operations (configuration)
    public:
        //  An unordered list of all "shared folders" configured for this SCP
        QList<SharedFolder*>sharedFolders() const { return _sharedFolders.values(); }

        //  Creates a new SharedFolder with the specified properties.
        //  If a SharedFolder with the specified "volumeName" already exists, it
        //  is "modified" instead.
        //  If the "volumeName" is not a valid SCP volume name, the call returns "nullptr".
        SharedFolder *      createSharedFolder(const QString & volumeName, const QString & hostPath);

        //  Destroys a SharedFolder with the specified "volumeName".
        //  Has no effect if a SharedFolder with the specified "volumeName" does not exist.
        //  If the "volumeName" is not a valid SCP volume name, the call has no effect.
        void                destroySharedFolder(const QString & volumeName);

        //  Modifies the "hostPath" of an existing SharedFolder.
        //  If the "volumeName" is not a valid SCP volume name OR the SharedFolder with that
        //  "volumeName" does not exist, the call has no effect.
        void                modifySharedFolder(const QString & volumeName, const QString & hostPath);

        //////////
        //  Operations
    public:
        //  Checks if the caller is running on the SCP worker (kernel) thread
        bool                onKernelThread() const;
        ErrorCode           makeSystemCall(SystemCall * systemCall);

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        ScpEditorList           _editors;   //  ...that have been created so far

        //  Configuration
        QMap<QString, SharedFolder*>    _sharedFolders; //  volume name -> shared folder definition

        //  Runtime state
        QMap<uint16_t, ibm360::Device*> _hardwareDevices;   //  Keyed by I/O address, populated by "connect()"
        QMap<Device*, DeviceDriver*>    _deviceDrivers;     //  Drivers to use for Devices, populated by "initialise()"
        PhysicalDevice *                _operatorsConsole = nullptr;

        //  Subsystems
        ObjectManager       _objectManager;

        QMap<Process*,SystemCall*>  _systemCallsInProgress;
        QMap<Device*,Process*>      _ioInProgress;

        //  Helpers
        void                _registerHardwareDevice(ibm360::Device * hardwareDevice);   //  throws VirtualApplianceException on error
        void                _createDevicesAndDeviceDrivers();   //  throws VirtualApplianceException on error
        void                _destroyDevicesAndDeviceDrivers();
        static bool         _isTerminal(ibm360::Device * hardwareDevice);

        //////////
        //  Interrupt handling
        class _TransferCompletionListener : public DeviceDriver::TransferCompletionListener
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_TransferCompletionListener)
        public:
            _TransferCompletionListener(Scp * scp) : _scp(scp) {}
        public:
            virtual void        transferCompleted(Device * device, uint32_t bytesTransferred, ErrorCode errorCode) override;
        private:
            Scp *const          _scp;
        };
        _TransferCompletionListener _transferCompletionListener;

        //////////
        //  Event handling
    private:
        class _Event
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Event)
        public:
            _Event(Process * process) : _process(process) {}
            virtual ~_Event() {}

            Process *const  _process;   //  can be nullptr!
        };

        class _SystemCallEvent : public _Event
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_SystemCallEvent)
        public:
            _SystemCallEvent(SystemCall * systemCall)
                :   _Event(systemCall->process()), _systemCall(systemCall) {}
            virtual ~_SystemCallEvent() {}

            SystemCall *const   _systemCall;
        };

        class _TransferCompleteEvent : public _Event
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_TransferCompleteEvent)
        public:
            _TransferCompleteEvent(Device * device, uint32_t bytesTransferred, ErrorCode errorCode)
                :   _Event(nullptr), _device(device), _bytesTransferred(bytesTransferred), _errorCode(errorCode) {}
            virtual ~_TransferCompleteEvent() {}

            Device *const   _device;
            const uint32_t  _bytesTransferred;
            const ErrorCode _errorCode;
        };

        util::BlockingQueue<_Event*>    _events;

        void                _handleSystemCallEvent(_SystemCallEvent * event);
        void                _handleWriteToOperatorSystemCall(WriteToOperatorSystemCall * systemCall);
        void                _handleReadFromOperatorSystemCall(ReadFromOperatorSystemCall * systemCall);
        void                _handleOpenFileSystemCall(OpenFileSystemCall * systemCall);
        void                _handleWriteToFileSystemCall(WriteToFileSystemCall * systemCall);
        void                _handleReadFromFileSystemCall(ReadFromFileSystemCall * systemCall);
        void                _handleSetEnvironmentVariableValueSystemCall(SetEnvironmentVariableValueSystemCall * systemCall);
        void                _handleUnknownSystemCall(SystemCall * systemCall);
        void                _handleTransferCompleteEvent(_TransferCompleteEvent * event);

        //////////
        //  Threads
    private:
        class _WorkerThread : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Construction/destruction
        public:
            explicit _WorkerThread(Scp * scp);
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
