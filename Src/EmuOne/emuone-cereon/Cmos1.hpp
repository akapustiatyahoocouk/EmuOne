//
//  emuone-cereon/Cmos1.hpp
//
//  Cereon CMOS1
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  The Cereon CMOS1 bundle
    class EMUONE_CEREON_EXPORT Cmos1 : public core::Component,
                                       public virtual IIoBusClient
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Cmos1)

        //////////
        //  Types
    public:
        //  The type of the CMOS1 component
        class EMUONE_CEREON_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  util::StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;

            //////////
            //  core::ComponentType
        public:
            virtual core::ComponentCategory *   category() const override;
            virtual bool                isCompatibleWith(core::Architecture * architecture) const override;
            virtual core::Component *   createComponent() override;
        };

        //////////
        //  Construction/destruction - from derived singleton classes only
    protected:
        explicit Cmos1(const QString & name);
        virtual ~Cmos1();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;

        //////////
        //  core::Component (state control) - all thread-safe
    public:
        virtual State           state() const override;
        virtual void            connect() override;      //  throws VirtualApplianceException
        virtual void            initialise() override;   //  throws VirtualApplianceException;
        virtual void            start() override;        //  throws VirtualApplianceException
        virtual void            stop() noexcept override;
        virtual void            deinitialise() noexcept override;
        virtual void            disconnect() noexcept override;

        //////////
        //  core::Component (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  IIoBusClient
    public:
        virtual IoPortList      ioPorts() override;

        //////////
        //  Operations
    public:
        uint16_t                statePortAddress() const { return _statePortAddress; }
        uint16_t                addressPortAddress() const { return _addressPortAddress; }
        uint16_t                dataPortAddress() const { return _dataPortAddress; }
        uint16_t                interruptMaskPortAddress() const { return _interruptMaskPortAddress; }
        QString                 savedStateFileName() const { return _savedStateFileName; }

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        //  Configuration
        uint16_t                _statePortAddress = 0x0110;
        uint16_t                _addressPortAddress = 0x0111;
        uint16_t                _dataPortAddress = 0x0112;
        uint16_t                _interruptMaskPortAddress = 0x0113;
        QString                 _savedStateFileName = "cmos.bin";

        //  Runtime state
        uint8_t                 _data[256];
        uint8_t                 _currentAddress = 0;    //  ...within the "_data" array to access
        uint8_t                 _interruptMask = 0;

        //////////
        //  I/O ports
        class _StatePort : public IByteIoPort
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_StatePort)

            //////////
            //  Construction/destruction
        public:
            explicit _StatePort(Cmos1 * cmos1) : _cmos1(cmos1) {}
            virtual ~_StatePort() {}

            //////////
            //  IIoPort
        public:
            virtual uint16_t    address() const override { return _cmos1->_statePortAddress; }

            //////////
            //  IByteIoPort
        public:
            virtual uint8_t     readByte() override;
            virtual void        writeByte(uint8_t /*value*/) override {}

            //////////
            //  Implementation
        private:
            Cmos1 *             _cmos1;
        };
        _StatePort              _statePort;

        class _AddressPort : public IByteIoPort
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_AddressPort)

            //////////
            //  Construction/destruction
        public:
            explicit _AddressPort(Cmos1 * cmos1) : _cmos1(cmos1) {}
            virtual ~_AddressPort() {}

            //////////
            //  IIoPort
        public:
            virtual uint16_t    address() const override { return _cmos1->_addressPortAddress; }

            //////////
            //  IByteIoPort
        public:
            virtual uint8_t     readByte() override;
            virtual void        writeByte(uint8_t value) override;

            //////////
            //  Implementation
        private:
            Cmos1 *             _cmos1;
        };
        _AddressPort            _addressPort;

        class _DataPort : public IByteIoPort
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_DataPort)

            //////////
            //  Construction/destruction
        public:
            explicit _DataPort(Cmos1 * cmos1) : _cmos1(cmos1) {}
            virtual ~_DataPort() {}

            //////////
            //  IIoPort
        public:
            virtual uint16_t    address() const override { return _cmos1->_dataPortAddress; }

            //////////
            //  IByteIoPort
        public:
            virtual uint8_t     readByte() override;
            virtual void        writeByte(uint8_t value) override;

            //////////
            //  Implementation
        private:
            Cmos1 *             _cmos1;
        };
        _DataPort               _dataPort;

        class _InterruptMaskPort : public IByteIoPort
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_InterruptMaskPort)

            //////////
            //  Construction/destruction
        public:
            explicit _InterruptMaskPort(Cmos1 * cmos1) : _cmos1(cmos1) {}
            virtual ~_InterruptMaskPort() {}

            //////////
            //  IIoPort
        public:
            virtual uint16_t    address() const override { return _cmos1->_dataPortAddress; }

            //////////
            //  IByteIoPort
        public:
            virtual uint8_t     readByte() override;
            virtual void        writeByte(uint8_t value) override;

            //////////
            //  Implementation
        private:
            Cmos1 *             _cmos1;
        };
        _InterruptMaskPort      _interruptMaskPort;
    };
}

//  End of emuone-cereon/Cmos1.hpp
