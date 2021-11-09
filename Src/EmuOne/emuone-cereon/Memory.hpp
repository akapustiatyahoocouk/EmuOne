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
    class EMUONE_CEREON_EXPORT RamUnit : public core::ResidentMemoryUnit<64, 8>
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
        RamUnit();
        virtual ~RamUnit();

        //////////
        //  core::Component
    public:
        virtual core::ComponentType *   type() const override;
    };

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
}

//  End of emuone-cereon/Memory.hpp
