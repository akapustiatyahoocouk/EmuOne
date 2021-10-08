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
    };
}

//  End of emuone-scp360/Scp.hpp
