//
//  emuone-cereon/Processors.hpp
//
//  Cereon processors
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  The "$state" register (accessor helpers)
    class EMUONE_CEREON_EXPORT StateRegister final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StateRegister)

        //////////
        //  Construction/destruction
    public:
        explicit StateRegister(uint64_t & value) : _value(value) {}
        ~StateRegister() {}

        //////////
        //  Operators
    public:
        StateRegister &     operator = (uint64_t value) { _value = value; return *this; }
                            operator uint64_t() const { return _value; }

        //////////
        //  Operations
    public:
        bool                inKernelMode() const { return (_value & UINT64_C(0x0000000000000001)) != 0; }
        bool                inUserMode() const { return (_value & UINT64_C(0x0000000000000001)) == 0; }
        void                setKernelMode() { _value |= UINT64_C(0x0000000000000001); }
        void                setUserMode() { _value &= UINT64_C(0xFFFFFFFFFFFFFFFE); }
        bool                inVirtualMode() const { return (_value & UINT64_C(0x0000000000000002)) != 0; }
        bool                inRealMode() const { return (_value & UINT64_C(0x0000000000000002)) == 0; }
        bool                inTrapMode() const { return (_value & UINT64_C(0x0000000000000004)) != 0; }
        bool                inPendingTrapMode() const { return (_value & UINT64_C(0x0000000000000008)) == 0; }

        //  Gets/sets the "byte order" currently indicated by this $state register
        util::ByteOrder     byteOrder() const { return ((_value & UINT64_C(0x0000000000000010)) != 0) ? util::ByteOrder::BigEndian : util::ByteOrder::LittleEndian; }
        void                setByteOrder(util::ByteOrder byteOrder)
        {
            _value =
                (byteOrder == util::ByteOrder::BigEndian) ?
                    (_value | UINT64_C(0x0000000000000010)) :
                    (_value & UINT64_C(0xFFFFFFFFFFFFFFEF));
        }

        bool                inWorkingMode() const { return (_value & UINT64_C(0x0000000000000020)) != 0; }
        bool                inIdleMode() const { return (_value & UINT64_C(0x0000000000000020)) == 0; }
        void                setWorkingMode() { _value |= UINT64_C(0x0000000000000020); }
        bool                debugEventsEnabled() const { return (_value & UINT64_C(0x0000000000000040)) != 0; }
        bool                realOperandExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000000080)) != 0; }
        bool                realDivisionByZeroExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000000100)) != 0; }
        bool                realOverflowExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000000200)) != 0; }
        bool                realUnderflowExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000000400)) != 0; }
        bool                realInexactExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000000800)) != 0; }
        bool                integerDivisionByZeroExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000001000)) != 0; }
        bool                integerOverflowExceptionsEnabled() const { return (_value & UINT64_C(0x0000000000002000)) != 0; }

        int                 interruptMask() const { return static_cast<int>(_value >> 26) & 0x3F; }
        bool                isTimerInterruptEnabled() const { return (_value & UINT64_C(0x0000000004000000)) != 0; }
        bool                isIoInterruptEnabled() const { return (_value & UINT64_C(0x0000000008000000)) != 0; }
        bool                isSvcInterruptEnabled() const { return (_value & UINT64_C(0x0000000010000000)) != 0; }
        bool                isProgramInterruptEnabled() const { return (_value & UINT64_C(0x0000000020000000)) != 0; }
        bool                isExternalInterruptEnabled() const { return (_value & UINT64_C(0x0000000040000000)) != 0; }
        void                enableExternalInterrupt() const { _value |= UINT64_C(0x0000000040000000); }
        bool                isHardwareInterruptEnabled() const { return (_value & UINT64_C(0x0000000080000000)) != 0; }
        void                enableHardwareInterrupt() const { _value |= UINT64_C(0x0000000080000000); }

        uint32_t            contextId() const { return static_cast<uint32_t>(_value >> 32); }

        //////////
        //  Implementation
    private:
        uint64_t &          _value; //  raw
    };

    //////////
    //  The "$flags" register
    class EMUONE_CEREON_EXPORT FlagsRegister final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(FlagsRegister)

        //////////
        //  Construction/destruction
    public:
        explicit FlagsRegister(uint64_t value) : _value(value) {}
        ~FlagsRegister() {}

        //////////
        //  Operators
    public:
        FlagsRegister &     operator = (uint64_t value) { _value = value; return *this; }

        //////////
        //  Operations
    public:
        void                setRealOperandFlag() { _value |= 0x0001; }
        void                setRealDivisionByZeroFlag() { _value |= 0x0002; }
        void                setRealOverflowFlag() { _value |= 0x0004; }
        void                setRealUnderflowFlag() { _value |= 0x0008; }
        void                setRealInexactFlag() { _value |= 0x0008; }
        void                setIntegerDivisionByZeroFlag() { _value |= 0x0010; }
        void                setIntegerOverflowFlag() { _value |= 0x0010; }

        uint64_t            value() const { return _value; }
        void                setValue(uint64_t value) { _value = value; }

        //////////
        //  Implementation
    private:
        uint64_t            _value = 0; //  raw
    };

    //////////
    //  A generic Cereon processor
    class EMUONE_CEREON_EXPORT Processor : public core::Component,
                                           public virtual core::IClockedComponentAspect
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor)

        //////////
        //  Construction/destruction - from derived singleton classes only
    protected:
        Processor(const QString & name, Features features, InstructionSet * instructionSet,
                  core::ClockFrequency clockFrequency, util::ByteOrder byteOrder,
                  uint8_t processorId, bool primary, uint64_t bootstrapIp);
        virtual ~Processor();

    protected:
        //  Adds the specified "core" to this processors; intended to be called
        //  from the derived class' constructors.
        //  Throws "VirtualApplianceException" if an error occurs.
        void                addCore(ProcessorCore * core);

        //////////
        //  core::Component (state control) - all thread-safe
    public:
        virtual State       state() const override;
        virtual void        connect() override;      //  throws VirtualApplianceException
        virtual void        initialise() override;   //  throws VirtualApplianceException;
        virtual void        start() override;        //  throws VirtualApplianceException
        virtual void        stop() noexcept override;
        virtual void        deinitialise() noexcept override;
        virtual void        disconnect() noexcept override;

        //////////
        //  core::Component (serialisation)
    public:
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  core::IClockedComponentAspect
    public:
        virtual core::ClockFrequency    clockFrequency() const override;
        virtual void                    onClockTick() override;

        //////////
        //  Properties
    public:
        Features                features() const { return _features; }
        InstructionSet *        instructionSet() const { return _instructionSet; }
        void                    setClockFrequency(const core::ClockFrequency & clockFrequency) { _clockFrequency = clockFrequency; }
        util::ByteOrder         byteOrder() const { return _byteOrder; }
        void                    setByteOrder(util::ByteOrder byteOrder) { _byteOrder = byteOrder; }
        uint8_t                 processorId() const { return _processorId; }
        void                    setProcessorId(uint8_t processorId) { _processorId = processorId; }
        bool                    primary() const { return _primary; }
        void                    setPrimary(bool primary) { _primary = primary; }
        uint64_t                bootstrapIp() const { return _bootstrapIp; }
        void                    setBootstrapIp(uint64_t bootstrapIp) { _bootstrapIp = bootstrapIp; }

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        QMap<uint8_t, ProcessorCore*>   _mapIdsToCores = {};

        //  Configuration
        Features                _features;
        InstructionSet *        _instructionSet;
        core::ClockFrequency    _clockFrequency;
        util::ByteOrder         _byteOrder; //  Mirrors "byte order" bit of $state for faster access
        uint8_t                 _processorId;
        bool                    _primary;
        uint64_t                _bootstrapIp;

        //  Connections to other VA components
        MemoryBus *             _memoryBus = nullptr;
        IoBus *                 _ioBus = nullptr;

        //  Runtime state
        int                     _numCores = 0;
        ProcessorCore **        _cores = nullptr;   //  array of "_numCores" elements
    };

    //////////
    //  A generic Cereon processor core.
    //  Note that a "processor core" is not a component by itself - it only exists
    //  in the scope of containing Processor
    class EMUONE_CORE_EXPORT ProcessorCore
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ProcessorCore)

        //////////
        //  Construction/destruction
    public:
        ProcessorCore(Processor * processor, uint8_t coreId, bool primary);
        virtual ~ProcessorCore();

        //////////
        //  Operations
    public:
        Processor *             processor() const { return _processor; }
        uint8_t                 coreId() const { return _coreId; }
        bool                    primary() const { return _primary; }

        void                    initialise();   //  throws VirtualApplianceException;
        void                    start();        //  throws VirtualApplianceException
        void                    stop() noexcept;
        void                    deinitialise() noexcept;

        //  Called once on every tick of the Processor's master clock
        void                    onClockTick();

        //////////
        //  Registers
    public:
        struct EMUONE_CEREON_EXPORT Registers final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Registers)

        public:
            Registers()
                //  Raw register values
                :   r(),
                    c(),
                    flags(0),
                    d(),
                    m(),
                    //  Registers by role
                    ip(r[0]),
                    state(c[0]),
                    pth(c[1]),
                    itc(c[2]),
                    cc(c[3]),
                    isaveipTm(c[4]),
                    isavestateTm(c[5]),
                    ihstateTm(c[6]),
                    ihaTm(c[7]),
                    isaveipIo(c[8]),
                    isavestateIo(c[9]),
                    ihstateIo(c[10]),
                    ihaIo(c[11]),
                    iscIo(c[12]),
                    isaveipSvc(c[13]),
                    isavestateSvc(c[14]),
                    ihstateSvc(c[15]),
                    ihaSvc(c[16]),
                    isaveipPrg(c[17]),
                    isavestatePrg(c[18]),
                    ihstatePrg(c[19]),
                    ihaPrg(c[20]),
                    iscPrg(c[21]),
                    isaveipExt(c[22]),
                    isavestateExt(c[23]),
                    ihstateExt(c[24]),
                    ihaExt(c[25]),
                    iscExt(c[26]),
                    isaveipHw(c[27]),
                    isavestateHw(c[28]),
                    ihstateHw(c[29]),
                    ihaHw(c[30]),
                    iscHw(c[31])
            {
            }

            ~Registers()
            {
            }

            //  Raw register values
            uint64_t            r[32];
            uint64_t            c[32];
            FlagsRegister       flags;
            uint64_t            d[32];
            uint64_t            m[32];

            //  Registers by role
            uint64_t &          ip;

            StateRegister       state;
            uint64_t &          pth;
            uint64_t &          itc;
            uint64_t &          cc;
            uint64_t &          isaveipTm;
            uint64_t &          isavestateTm;
            StateRegister       ihstateTm;
            uint64_t &          ihaTm;
            uint64_t &          isaveipIo;
            StateRegister       isavestateIo;
            StateRegister       ihstateIo;
            uint64_t &          ihaIo;
            uint64_t &          iscIo;
            uint64_t &          isaveipSvc;
            StateRegister       isavestateSvc;
            StateRegister       ihstateSvc;
            uint64_t &          ihaSvc;
            uint64_t &          isaveipPrg;
            StateRegister       isavestatePrg;
            StateRegister       ihstatePrg;
            uint64_t &          ihaPrg;
            uint64_t &          iscPrg;
            uint64_t &          isaveipExt;
            StateRegister       isavestateExt;
            StateRegister       ihstateExt;
            uint64_t &          ihaExt;
            uint64_t &          iscExt;
            uint64_t &          isaveipHw;
            StateRegister       isavestateHw;
            StateRegister       ihstateHw;
            uint64_t &          ihaHw;
            uint64_t &          iscHw;
        };
        Registers               registers;

        //////////
        //  Implementation
    private:
        Processor *const        _processor; //  ...to which this core belongs
        const uint8_t           _coreId;    //  unique ID of the core within the Processor
        const bool              _primary;
    };

    //////////
    //  The Cereon-1P1B processor
    class EMUONE_CEREON_EXPORT Processor1P1B : public Processor
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor1P1B)

        //////////
        //  Types
    public:
        //  The type of the Cereon-1P1B processor
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
        Processor1P1B(const QString & name, uint8_t processorId, bool primary, uint64_t bootstrapIp);
        virtual ~Processor1P1B();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;

        //////////
        //  Implementation
    private:
        ProcessorCore                   _core;
    };
}

//  End of emuone-cereon/Processors.hpp
