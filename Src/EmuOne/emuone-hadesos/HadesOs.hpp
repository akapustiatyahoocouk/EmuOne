//
//  emuone-hadesos/HadesOs.hpp
//
//  The HADES OS component
//
//////////
#pragma once

namespace hadesos
{
    //////////
    //  A HADES OS
    class EMUONE_HADESOS_EXPORT HadesOs : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HadesOs)

        //////////
        //  Types
    public:
        //  The "type" of "HADES OS" component
        class EMUONE_HADESOS_EXPORT Type : public core::ComponentType
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
            virtual bool        isCompatibleWith(const core::Architecture * architecture) const override;
            virtual HadesOs *   createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        explicit HadesOs(const QString & name);
        virtual ~HadesOs();

        //////////
        //  Component
    public:
        virtual Type *                  type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;

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
        mutable QRecursiveMutex _stateGuard = {};

        //ScpEditorList           _editors = {};  //  ...that have been created so far
    };
}

//  End of emuone-hadesos/HadesOs.hpp
