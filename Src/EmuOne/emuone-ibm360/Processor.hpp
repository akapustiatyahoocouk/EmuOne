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
    //  The IBM/360 processor
    class EMUONE_IBM360_EXPORT Processor : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor)

        friend class ::Ibm360ProcessorEditor;

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
        virtual core::ComponentType*type() const override;
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual core::ComponentUi * createUi() override;

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
        mutable QRecursiveMutex _stateGuard;

        //  Component configuration
        Features                _features;
        core::ClockFrequency    _clockFrequency;
    };
}

//  End of emuone-ibm360/Processor.hpp
