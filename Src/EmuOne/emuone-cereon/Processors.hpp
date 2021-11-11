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
    //  A generic Cereon processor
    class EMUONE_CEREON_EXPORT Processor : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor)

        //////////
        //  Construction/destruction - from derived singleton classes only
    protected:
        Processor(const QString & name, Features features, InstructionSet * instructionSet,
                  core::ClockFrequency clockFrequency, util::ByteOrder byteOrder);
        virtual ~Processor();

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
        //  Properties
    public:
        Features                features() const { return _features; }
        InstructionSet *        instructionSet() const { return _instructionSet; }
        core::ClockFrequency    clockFrequency() const { return _clockFrequency; }
        util::ByteOrder         byteOrder() const { return _byteOrder; }

        //////////
        //  Registers
    public:
        uint64_t                r[32] = {};
        uint64_t                c[32] = {};
        uint64_t                flags = {};
        uint64_t                d[32] = {};
        uint64_t                m[32] = {};

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        //  Configuration
        Features                _features;
        InstructionSet *        _instructionSet;
        core::ClockFrequency    _clockFrequency;
        util::ByteOrder         _byteOrder;

        //  Connections to other VA components
        MemoryBus *             _memoryBus = nullptr;
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
        Processor1P1B();
        virtual ~Processor1P1B();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;
    };
}

//  End of emuone-cereon/Processors.hpp
