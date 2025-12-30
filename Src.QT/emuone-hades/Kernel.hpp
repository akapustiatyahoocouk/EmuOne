//
//  emuone-hades/Kernel.hpp - The HADES OS Kernel
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::hades
{
    /// \class Kernel emuone-hades/API.hpp
    /// \brief The HADES OS Kernel.
    class EMUONE_HADES_PUBLIC Kernel final
        :   public virtual emuone::core::IComponent
    {
        //////////
        //  Types
    public:
        class EMUONE_HADES_PUBLIC Type final
            :   public virtual emuone::core::IComponentType
        {
            EMUONE_DECLARE_SINGLETON(Type)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;

            //////////
            //  emuone::core::IComponentType
        public:
            virtual auto    category() const -> emuone::core::IComponentCategory * override;
            virtual bool    isCompatibleWith(emuone::core::IArchitecture * architecture) const override;
            virtual bool    isCompatibleWith(emuone::core::IVirtualMachineType * virtualMachineType) const override;
            virtual bool    isPersistable() const override;
            virtual auto    createComponent() -> Kernel * override;
        };

        //////////
        //  Construction/destruction
    public:
        Kernel();
        virtual ~Kernel();

        //////////
        //  emuone::core::IComponent
    public:
        virtual auto    type() const -> emuone::core::IComponentType * override;
        virtual QString displayName() const override;
        virtual auto    createEditor(QWidget * parent) -> emuone::core::ComponentEditor * override;
        virtual void    saveConfiguration(QDomElement & element) const override;
        virtual void    restoreConfiguration(const QDomElement & element) override;

        //////////
        //  emuone::core::IComponent (state control)
    public:
        virtual State   state() const override;
        virtual void    connect() override;
        virtual void    disconnect() noexcept override;
        virtual void    initialize() override;
        virtual void    deinitialize() noexcept override;
        virtual void    start() override;
        virtual void    stop() noexcept override;

        //////////
        //  Implementation
    private:
        State           _state = State::Constructed;
    };
}

//  End of emuone-hades/Kernel.hpp
