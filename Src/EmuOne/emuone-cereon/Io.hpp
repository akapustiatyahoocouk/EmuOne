//
//  emuone-cereon/Io.hpp
//
//  Cereon I/O
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  A generic "I/O port" handles CPU's accesses to a single I/O bus address
    class EMUONE_CEREON_EXPORT IIoPort
    {
        //////////
        //  This is an interface
    public:
        virtual ~IIoPort() {}

        //////////
        //  Operations
    public:
        //  The address of this I/O port.
        virtual uint16_t        address() const = 0;

        //  True if this I/O port is allowed to generate I/O interrupts
        bool                    canInterrupt() const { return _canInterrupt; }

        //  True if an interrupt is currently pending in this I/O port
        bool                    interruptPending() const { return _interruptPending; }

        //////////
        //  Implementation
    private:
        bool                    _canInterrupt = false;
        bool                    _interruptPending = 0;
        uint16_t                _interruptStatusCode = 0;
    };
    using IoPortList = QList<IIoPort*>;

    //////////
    //  A byte I/O port
    class EMUONE_CEREON_EXPORT IByteIoPort : public virtual IIoPort
    {
        //////////
        //  This is an interface
    public:
        virtual ~IByteIoPort() {}

        //////////
        //  Operations
    public:
        virtual uint8_t         readByte() = 0;
        virtual void            writeByte(uint8_t value) = 0;
    };

    //////////
    //  A half-word I/O port
    class EMUONE_CEREON_EXPORT IHalfWordIoPort : public virtual IIoPort
    {
        //////////
        //  This is an interface
    public:
        virtual ~IHalfWordIoPort() {}

        //////////
        //  Operations
    public:
        virtual uint16_t        readHalfWord() = 0;
        virtual void            writeHalfWord(uint16_t value) = 0;
    };

    //////////
    //  A word I/O port
    class EMUONE_CEREON_EXPORT IWordIoPort : public virtual IIoPort
    {
        //////////
        //  This is an interface
    public:
        virtual ~IWordIoPort() {}

        //////////
        //  Operations
    public:
        virtual uint32_t        readWord() = 0;
        virtual void            writeWord(uint32_t value) = 0;
    };

    //////////
    //  A long word I/O port
    class EMUONE_CEREON_EXPORT ILongWordIoPort : public virtual IIoPort
    {
        //////////
        //  This is an interface
    public:
        virtual ~ILongWordIoPort() {}

        //////////
        //  Operations
    public:
        virtual uint64_t        readLongWord() = 0;
        virtual void            writeLongWord(uint64_t value) = 0;
    };

    //////////
    //  An interface to a component that responds to I/O bus accesses
    class EMUONE_CEREON_EXPORT IIoBusClient
    {
        //////////
        //  This is an interface
    public:
        virtual ~IIoBusClient() {}

        //////////
        //  Operations
    public:
        //  Returns an unordered list of all "I/O ports" provided by the component
        virtual IoPortList      ioPorts() = 0;
    };

    //////////
    //  A Cereon I/O bus
    class EMUONE_CEREON_EXPORT IoBus : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IoBus)

        //////////
        //  Types
    public:
        //  The type of the Cereon I/O bus
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
            virtual bool                        isCompatibleWith(const core::Architecture * architecture) const override;
            virtual core::Component *           createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        explicit IoBus(const QString & name);
        virtual ~IoBus();

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
        //  Operations
    public:
        //  Port I/O - inline for better performance
        uint8_t                 readByte(uint16_t address)
        {
            QMutexLocker lock(&_accessGuard);
            if (_byteIoPorts.contains(address))
            {
                return _byteIoPorts[address]->readByte();
            }
            return 0;
        }

        void                    writeByte(uint16_t address, uint8_t value)
        {
            QMutexLocker lock(&_accessGuard);
            if (_byteIoPorts.contains(address))
            {
                _byteIoPorts[address]->writeByte(value);
            }
        }

        uint16_t                readHalfWord(uint16_t address)
        {
            QMutexLocker lock(&_accessGuard);
            if (_halfWordIoPorts.contains(address))
            {
                return _halfWordIoPorts[address]->readHalfWord();
            }
            return 0;
        }

        void                    writeHalfWord(uint16_t address, uint16_t value)
        {
            QMutexLocker lock(&_accessGuard);
            if (_halfWordIoPorts.contains(address))
            {
                _halfWordIoPorts[address]->writeHalfWord(value);
            }
        }

        uint32_t                readWord(uint16_t address)
        {
            QMutexLocker lock(&_accessGuard);
            if (_wordIoPorts.contains(address))
            {
                return _wordIoPorts[address]->readWord();
            }
            return 0;
        }

        void                    writeWord(uint16_t address, uint32_t value)
        {
            QMutexLocker lock(&_accessGuard);
            if (_wordIoPorts.contains(address))
            {
                _wordIoPorts[address]->writeWord(value);
            }
        }

        uint64_t                readLongWord(uint16_t address)
        {
            QMutexLocker lock(&_accessGuard);
            if (_longWordIoPorts.contains(address))
            {
                return _longWordIoPorts[address]->readLongWord();
            }
            return 0;
        }

        void                    writeLongWord(uint16_t address, uint64_t value)
        {
            QMutexLocker lock(&_accessGuard);
            if (_longWordIoPorts.contains(address))
            {
                _longWordIoPorts[address]->writeLongWord(value);
            }
        }

        //  Implements the tstp/setp instruction for the specified I/O address
        uint64_t                getPostStatus(uint16_t address);
        void                    setPostStatus(uint16_t address, uint64_t postStatus);

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        //  Connections to other VM components
        QMap<uint16_t, IIoPort*>            _ioPorts = {};          //  all I/O ports
        QMap<uint16_t, IByteIoPort*>        _byteIoPorts = {};      //  those "_ioPorts" that are also IByteIoPorts
        QMap<uint16_t, IHalfWordIoPort*>    _halfWordIoPorts = {};  //  those "_ioPorts" that are also IHalfWordIoPorts
        QMap<uint16_t, IWordIoPort*>        _wordIoPorts = {};      //  those "_ioPorts" that are also IWordIoPorts
        QMap<uint16_t, ILongWordIoPort*>    _longWordIoPorts = {};  //  those "_ioPorts" that are also ILongWordIoPorts

        QMutex                  _accessGuard = {};  //  to make sure only one I/O is happenning at a time
    };
}

//  End of emuone-cereon/Io.hpp
