//
//  emuone-ibm360/Processor.hpp
//
//  IBM/360 processor
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  The IBM/360 PSW
    class EMUONE_IBM360_EXPORT Psw final
    {
        //////////
        //  Construction/destruction/assignment
    public:
        Psw() : _value(0) {}

        //////////
        //  Operations
    public:
        uint8_t             systemMask() const { return static_cast<uint8_t>(_value >> 56); }
        uint8_t             protectionKey() { return static_cast<uint8_t>((_value >> 52) & 0x0F); }
        bool                asciiMode() const { return (_value & UINT64_C(0x0008000000000000)) != 0; }
        bool                ebcdicMode() const { return (_value & UINT64_C(0x0008000000000000)) == 0; }
        bool                machineCheckMode() const { return (_value & UINT64_C(0x0004000000000000)) != 0; }
        bool                waitState() const { return (_value & UINT64_C(0x0002000000000000)) != 0; }
        bool                runningState() const { return (_value & UINT64_C(0x0002000000000000)) == 0; }
        bool                problemState() const { return (_value & UINT64_C(0x0001000000000000)) != 0; }
        bool                supervisorState() const { return (_value & UINT64_C(0x0001000000000000)) == 0; }
        uint16_t            interruptionCode() const { return static_cast<uint16_t>(_value >> 32); }
        uint8_t             instructionLengthCode() const { return static_cast<uint8_t>((_value >> 30) & 0x03); }
        uint8_t             conditionCode() const { return static_cast<uint8_t>((_value >> 28) & 0x03); }
        uint8_t             programMask() const { return static_cast<uint8_t>((_value >> 24) & 0x03); }
        bool                fixedPointOverflow() const { return (_value & UINT64_C(0x0000000008000000)) != 0; }
        bool                decimalOverflow() const { return (_value & UINT64_C(0x0000000004000000)) != 0; }
        bool                exponentUnderflow() const { return (_value & UINT64_C(0x0000000002000000)) != 0; }
        bool                significance() const { return (_value & UINT64_C(0x0000000001000000)) != 0; }
        uint32_t            instructionAddress() const { return static_cast<uint32_t>(_value & 0x00FFFFFF); }
        void                setInstructionAddress(uint32_t instructionAddress) { _value = ((_value & UINT64_C(0xFFFFFFFFFF000000)) | (instructionAddress & 0x00FFFFFF)); }

        //////////
        //  Implementation
    private:
        uint64_t            _value;
    };

    //////////
    //  The IBM/360 processor
    class EMUONE_IBM360_EXPORT Processor : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor)

        friend class Ibm360ProcessorEditor;

        //////////
        //  Types
    public:
        //  The "type" of IBM/360 processor
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
            virtual Processor * createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        Processor(const QString & name, Features features, const core::ClockFrequency & clockFrequency);
        virtual ~Processor();

        //////////
        //  Component
    public:
        virtual core::ComponentType *   type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State           state() const override;
        virtual void            connect() override;
        virtual void            initialise() override;
        virtual void            start() override;
        virtual void            stop() noexcept override;
        virtual void            deinitialise() noexcept override;
        virtual void            disconnect() noexcept override;

        //////////
        //  Component (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        static bool             isValidClockFrequency(const core::ClockFrequency & clockFrequency);
        Features                features() const { return _features; }
        bool                    setFeatures(Features features);
        core::ClockFrequency    clockFrequency() const { return _clockFrequency; }
        bool                    setClockFrequency(const core::ClockFrequency & clockFrequency);

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        //  Component configuration
        Features                _features;
        core::ClockFrequency    _clockFrequency;

        //  Connection to other VM components
        Storage *               _storage = nullptr;
        Monitor *               _monitor = nullptr;

        //  Runtime state
        Psw                     _psw = {};
        bool                    _stopped = true;
        bool                    _iplInProgress = false;
    };
}

//  End of emuone-ibm360/Processor.hpp
