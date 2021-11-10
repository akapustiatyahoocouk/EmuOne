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
    class EMUONE_CEREON_EXPORT MemoryUnit
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryUnit)

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
        //  Construction/destruction
    public:
        MemoryUnit() {}
        virtual ~MemoryUnit() {}

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

    //////////
    //  A "memory unit" that starts at a known address
    class EMUONE_CEREON_EXPORT BoundMemoryUnit : public MemoryUnit
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BoundMemoryUnit)

        //////////
        //  Cobstruction/destruction
    public:
        BoundMemoryUnit() {}
        virtual ~BoundMemoryUnit() {}

        //////////
        //  Operations
    public:
        //  The "start address" of this memory unit in the address space.
        //  The memory unit covers the address range [<start address> ... <start address>+<size>)
        virtual uint64_t            startAddress() const = 0;
    };
    using BoundMemoryUnitList = QList<BoundMemoryUnit*>;

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
                                         public BoundMemoryUnit, public virtual IMemoryBusClient
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RamUnit)

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
            virtual bool                        isCompatibleWith(core::Architecture * architecture) const override;
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
        //  MemoryUnit
    public:
        virtual core::MemorySize    size() const override;
        virtual AccessOutcome       loadByte(uint64_t offset, uint8_t & value) override;
        virtual AccessOutcome       loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value) override;
        virtual AccessOutcome       loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value) override;
        virtual AccessOutcome       loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value) override;
        virtual AccessOutcome       storeByte(uint64_t offset, uint8_t value) override;
        virtual AccessOutcome       storeHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t value) override;
        virtual AccessOutcome       storeWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t value) override;
        virtual AccessOutcome       storeLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t value) override;

        //////////
        //  BoundMemoryUnit
    public:
        virtual uint64_t            startAddress() const override;

        //////////
        //  IMemoryBusClient
    public:
        virtual BoundMemoryUnitList memoryUnits() override;

        //////////
        //  Operations
    public:
        //  Sets the size of this RAM unit.
        virtual void                setSize(const core::MemorySize & size);

        //  Sets the start address of this RAM unit.
        virtual void                setStartAddress(uint64_t startAddress);

        //////////
        //  Implementation
    private:
        State                       _state = State::Constructed;
        mutable QRecursiveMutex     _stateGuard;

        //  Configuration
        core::MemorySize            _size;
        uint64_t                    _startAddress;

        //  Runtime state
        uint8_t *                   _data = nullptr;
        size_t                      _dataSize = 0;
    };

/*
    //////////
    //  Cereon ROM
    class EMUONE_CEREON_EXPORT RomUnit : public core::ResidentMemoryUnit<64, 8>
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RomUnit)

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
        //  Construction/destruction
    public:
        RomUnit();
        virtual ~RomUnit();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
    };
*/
}

//  End of emuone-cereon/Memory.hpp
