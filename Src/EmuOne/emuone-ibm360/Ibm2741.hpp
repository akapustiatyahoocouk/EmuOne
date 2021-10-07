//
//  emuone-ibm360/Ibm2741.hpp
//
//  IBM 2741 terminal
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  The IBM 2741 terminal
    class EMUONE_IBM360_EXPORT Ibm2741 : public Device
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm2741)

        //////////
        //  Types
    public:
        //  The "type" of IBM 2741 terminal
        class EMUONE_IBM360_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  StockObject
        public:
            virtual QString     getMnemonic() const override;
            virtual QString     getDisplayName() const override;

            //////////
            //  ComponentType
        public:
            virtual core::ComponentCategory *   getCategory() const override;
            virtual bool        isCompatibleWith(core::Architecture * architecture) const override;
            virtual Ibm2741 *   createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        Ibm2741(const QString & name, uint16_t address);
        virtual ~Ibm2741();

        //////////
        //  Component
    public:
        virtual core::ComponentType *   getType() const override;
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             getShortStatus() const override;
        virtual core::ComponentUi * createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State           getState() const override;
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

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;
    };
}

//  End of emuone-ibm360/Ibm2741.hpp
