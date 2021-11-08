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
    //  Cereon RAM
    class EMUONE_CEREON_EXPORT ResidentRamUnit : public core::ResidentRamUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRamUnit)

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
        ResidentRamUnit();
        virtual ~ResidentRamUnit();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
    };

    //////////
    //  Cereon ROM
    class EMUONE_CEREON_EXPORT ResidentRomUnit : public core::ResidentRomUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRomUnit)

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
        ResidentRomUnit();
        virtual ~ResidentRomUnit();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
    };
}

//  End of emuone-cereon/Memory.hpp
