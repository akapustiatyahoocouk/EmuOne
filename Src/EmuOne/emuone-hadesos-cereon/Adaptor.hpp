//
//  emuone-hadesos-cereon/Adaptor.hpp
//
//  HadesOS/Cereon Adaptor
//
//////////

namespace hadesos
{
    namespace cereon
    {
        //////////
        //  The HADES OS / Cereon adaptor
        class EMUONE_HADESOS_CEREON_EXPORT Adaptor : public core::Adaptor
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Adaptor)

            //////////
            //  Types
        public:
            //  The type of the HADES OS / Cerreon adaptor
            class EMUONE_CEREON_EXPORT Type : public core::AdaptorType
            {
                DECLARE_SINGLETON(Type)

                //////////
                //  util::StockObject
            public:
                virtual QString     mnemonic() const override;
                virtual QString     displayName() const override;

                //////////
                //  core::AdaptorType
            public:
                virtual bool        isCompatibleWith(core::Architecture * architecture) const override;
                virtual bool        isCompatibleWith(core::ComponentType * componentType) const override;
                virtual Adaptor *   createAdaptor(core::Component * adaptedComponent) override;
            };

            //////////
            //  Construction/destruction
        public:
            explicit Adaptor(HadesOs * hadesOs);
            virtual ~Adaptor();

            //////////
            //  core::Adaptor
        public:
            virtual core::AdaptorType * type() const override;
            virtual core::AdaptorEditor*createEditor(QWidget * parent) override;
            virtual QString             shortStatus() const override;
            virtual core::AdaptorUi *   createUi() override;

            //////////
            //  core::Adaptor (state control) - all thread-safe
        public:
            virtual State           state() const override;
            virtual void            connect() override;      //  throws VirtualApplianceException
            virtual void            initialise() override;   //  throws VirtualApplianceException;
            virtual void            start() override;        //  throws VirtualApplianceException
            virtual void            stop() noexcept override;
            virtual void            deinitialise() noexcept override;
            virtual void            disconnect() noexcept override;

            //////////
            //  core::Adaptor (serialisation)
        public:
            virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
            virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;
        };
    }
}

//  End of emuone-hadesos-cereon/Adaptor.hpp
