//
//  emuone-cereon/Memory.hpp
//
//  Cereon memory
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  A generic "memory unit" handles memory bus accesses for a continuous range
    //  of addresses
    class EMUONE_CEREON_EXPORT IMemoryUnit
    {
        //////////
        //  Types
    public:
        //  Possible outcomes of a memory access attempt
        enum class AccessOutcome
        {
            Success,            //  memory access succeeded
            InvalidAddress,     //  address is e.g. out of range
            InvalidAlignment,   //  address is e.g. mis-aligned
            AccessDenied,       //  access not wllowed (e.g. writing to ROM, etc.)
            UnknownError        //  unknown / hardware error
        };

        //////////
        //  This is an interface
    public:
        virtual ~IMemoryUnit() {}

        //////////
        //  Operations
    public:
        //  The size of this memory unit.
        virtual core::MemorySize    size() const = 0;

        //  Performs a load/store. IMPORTANT: Must be naturally aligned.
        virtual AccessOutcome       loadByte(uint64_t offset, uint8_t & value) = 0;
        virtual AccessOutcome       loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value) = 0;
        virtual AccessOutcome       loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value) = 0;
        virtual AccessOutcome       loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value) = 0;
        virtual AccessOutcome       storeByte(uint64_t offset, uint8_t value) = 0;
        virtual AccessOutcome       storeHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t value) = 0;
        virtual AccessOutcome       storeWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t value) = 0;
        virtual AccessOutcome       storeLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t value) = 0;
    };
    using MemoryUnitList = QList<IMemoryUnit*>;

    //////////
    //  A "memory unit" that starts at a known address
    class EMUONE_CEREON_EXPORT IBoundMemoryUnit : public virtual IMemoryUnit
    {
        //////////
        //  This is an interface
    public:
        virtual ~IBoundMemoryUnit() {}

        //////////
        //  Operations
    public:
        //  The "start address" of this memory unit in the address space.
        //  The memory unit covers the address range [<start address> ... <start address>+<size>)
        virtual uint64_t            startAddress() const = 0;
    };
    using BoundMemoryUnitList = QList<IBoundMemoryUnit*>;

    //////////
    //  An interface to a component that responds to memory bus accesses
    class EMUONE_CEREON_EXPORT IMemoryBusClient
    {
        //////////
        //  This is an interface
    public:
        virtual ~IMemoryBusClient() {}

        //////////
        //  Operations
    public:
        //  Returns an unordered list of all "memory units" provided by the component
        virtual BoundMemoryUnitList memoryUnits() = 0;
    };

    //////////
    //  Cereon RAM unit
    class EMUONE_CEREON_EXPORT RamUnit : public core::Component,
                                         public virtual IBoundMemoryUnit, public virtual IMemoryBusClient
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RamUnit)

        friend class RamUnitEditor;

        //////////
        //  Types
    public:
        //  The type of the Cereon resident RAM unit
        class EMUONE_CEREON_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  util::StockObject
        public:
            virtual QString         mnemonic() const override;
            virtual QString         displayName() const override;

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
        RamUnit(const QString & name, const core::MemorySize & size, uint64_t startAddress);
        virtual ~RamUnit();

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
        virtual State               state() const override;
        virtual void                connect() override;
        virtual void                initialise() override;
        virtual void                start() override;
        virtual void                stop() noexcept override;
        virtual void                deinitialise() noexcept override;
        virtual void                disconnect() noexcept override;

        //////////
        //  core::Component (serialisation)
    public:
        virtual void                serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void                deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  IMemoryUnit
    public:
        virtual core::MemorySize    size() const override { return _size; }
        virtual AccessOutcome       loadByte(uint64_t offset, uint8_t & value) override;
        virtual AccessOutcome       loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value) override;
        virtual AccessOutcome       loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value) override;
        virtual AccessOutcome       loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value) override;
        virtual AccessOutcome       storeByte(uint64_t offset, uint8_t value) override;
        virtual AccessOutcome       storeHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t value) override;
        virtual AccessOutcome       storeWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t value) override;
        virtual AccessOutcome       storeLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t value) override;

        //////////
        //  IBoundMemoryUnit
    public:
        virtual uint64_t            startAddress() const override { return _startAddress; }

        //////////
        //  IMemoryBusClient
    public:
        virtual BoundMemoryUnitList memoryUnits() override;

        //////////
        //  Operations
    public:
        //  Sets the size of this RAM unit.
        void                        setSize(const core::MemorySize & size) { _size = size; }

        //  Sets the start address of this RAM unit.
        void                        setStartAddress(uint64_t startAddress) { _startAddress = startAddress; }

        //////////
        //  Implementation
    private:
        State                       _state = State::Constructed;
        mutable QRecursiveMutex     _stateGuard = {};

        RamUnitEditorList           _editors = {};

        //  Configuration
        core::MemorySize            _size;
        uint64_t                    _startAddress;

        //  Runtime state
        uint8_t *                   _data = nullptr;
        size_t                      _dataSize = 0;
    };

    //////////
    //  Cereon ROM unit
    class EMUONE_CEREON_EXPORT RomUnit : public core::Component,
                                         public virtual IBoundMemoryUnit, public virtual IMemoryBusClient
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RomUnit)

        friend class RomUnitEditor;

        //////////
        //  Types
    public:
        //  The type of the Cereon resident ROM unit
        class EMUONE_CEREON_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  util::StockObject
        public:
            virtual QString         mnemonic() const override;
            virtual QString         displayName() const override;

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
        RomUnit(const QString & name, const core::MemorySize & size, uint64_t startAddress, const QString & contentFileName);
        virtual ~RomUnit();

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
        virtual State               state() const override;
        virtual void                connect() override;
        virtual void                initialise() override;
        virtual void                start() override;
        virtual void                stop() noexcept override;
        virtual void                deinitialise() noexcept override;
        virtual void                disconnect() noexcept override;

        //////////
        //  core::Component (serialisation)
    public:
        virtual void                serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void                deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  IMemoryUnit
    public:
        virtual core::MemorySize    size() const override { return _size; }
        virtual AccessOutcome       loadByte(uint64_t offset, uint8_t & value) override;
        virtual AccessOutcome       loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value) override;
        virtual AccessOutcome       loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value) override;
        virtual AccessOutcome       loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value) override;
        virtual AccessOutcome       storeByte(uint64_t offset, uint8_t value) override;
        virtual AccessOutcome       storeHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t value) override;
        virtual AccessOutcome       storeWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t value) override;
        virtual AccessOutcome       storeLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t value) override;

        //////////
        //  IBoundMemoryUnit
    public:
        virtual uint64_t            startAddress() const override { return _startAddress; }

        //////////
        //  IMemoryBusClient
    public:
        virtual BoundMemoryUnitList memoryUnits() override;

        //////////
        //  Operations
    public:
        //  Sets the size of this ROM unit.
        void                        setSize(const core::MemorySize & size) { _size = size; }

        //  Sets the start address of this ROM unit.
        void                        setStartAddress(uint64_t startAddress) { _startAddress = startAddress; }

        //  The name of the ROM content file
        QString                     contentFileName() const { return _contentFileName; }
        void                        setContentFileName(const QString & contentFileName) { _contentFileName = contentFileName; }

        //////////
        //  Implementation
    private:
        State                       _state = State::Constructed;
        mutable QRecursiveMutex     _stateGuard = {};

        RomUnitEditorList           _editors = {};

        //  Configuration
        core::MemorySize            _size;
        uint64_t                    _startAddress;
        QString                     _contentFileName;

        //  Runtime state
        uint8_t *                   _data = nullptr;
        size_t                      _dataSize = 0;
    };

    //////////
    //  The Cereon memory bus 64-bit address, 8/16/32/64-bit data
    class EMUONE_CEREON_EXPORT MemoryBus : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryBus)

        //////////
        //  Types
    public:
        //  The type of the Cereon memory bus
        class EMUONE_CEREON_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  util::StockObject
        public:
            virtual QString         mnemonic() const override;
            virtual QString         displayName() const override;

            //////////
            //  core::ComponentType
        public:
            virtual core::ComponentCategory *   category() const override;
            virtual bool                        isCompatibleWith(const core::Architecture * architecture) const override;
            virtual core::Component *           createComponent() override;
        };

        typedef IMemoryUnit::AccessOutcome AccessOutcome;

        //////////
        //  Construction/destruction
    public:
        explicit MemoryBus(const QString & name);
        virtual ~MemoryBus();

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
        virtual State               state() const override;
        virtual void                connect() override;
        virtual void                initialise() override;
        virtual void                start() override;
        virtual void                stop() noexcept override;
        virtual void                deinitialise() noexcept override;
        virtual void                disconnect() noexcept override;

        //////////
        //  core::Component (serialisation)
    public:
        virtual void                serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void                deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        AccessOutcome               loadByte(uint64_t address, uint8_t & value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->loadByte(address, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               loadHalfWord(uint64_t address, util::ByteOrder byteOrder, uint16_t & value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->loadHalfWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               loadWord(uint64_t address, util::ByteOrder byteOrder, uint32_t & value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->loadWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               loadLongWord(uint64_t address, util::ByteOrder byteOrder, uint64_t & value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->loadLongWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               storeByte(uint64_t address, uint8_t value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->storeByte(address, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               storeHalfWord(uint64_t address, util::ByteOrder byteOrder, uint16_t value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->storeHalfWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               storeWord(uint64_t address, util::ByteOrder byteOrder, uint32_t value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->storeWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        AccessOutcome               storeLongWord(uint64_t address, util::ByteOrder byteOrder, uint64_t value)
        {
            if (IMemoryUnit * memoryUnit = _findMemoryUnit(address))
            {   //  The "address" is now the offset within the "memoryUnit"
                return memoryUnit->storeLongWord(address, byteOrder, value);
            }
            return AccessOutcome::InvalidAddress;
        }

        //////////
        //  Implementation
    private:
        State                       _state = State::Constructed;
        mutable QRecursiveMutex     _stateGuard = {};

        //  Connections to other VM components (created by "connect()")
        struct _MemoryUnitMapping
        {
            uint64_t                _startAddress;  //  ...inclusive
            uint64_t                _endAddress;    //  ...inclusive
            IMemoryUnit *           _memoryUnit;    //  ...covering the range [_startAddress .. _endAddress]
        };
        size_t                      _numMemoryUnitMappings = 0;
        _MemoryUnitMapping *        _memoryUnitMappings = nullptr;  //  array of _numMemoryUnitMappings elements, sorted by "_startAddress"

        //  Helpers
        IMemoryUnit *               _findMemoryUnit(uint64_t & address) const
        {
            for (size_t i = 0; i < _numMemoryUnitMappings; i++)
            {
                _MemoryUnitMapping * mapping = _memoryUnitMappings + i;
                if (address >= mapping->_startAddress && address <= mapping->_endAddress)
                {
                    address -= mapping->_startAddress;
                    return mapping->_memoryUnit;
                }
            }
            return nullptr;
        }
    };
}

//  End of emuone-cereon/Memory.hpp
