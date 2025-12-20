//
//  emuone-hades/Dynaload.hpp - Dynamic loading support
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
    /// \class Component emuone-hades/API.hpp
    /// \brief the emuone-hades component.
    class EMUONE_HADES_PUBLIC Component final
        :   public virtual emuone::util::IComponent
    {
        EMUONE_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Settings emuone-util/API.hpp
        /// \brief The component's settings.
        class Settings : public emuone::util::Settings
        {
            EMUONE_DECLARE_SINGLETON(Settings)
        };

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QString mnemonic() const override;
        virtual QString displayName() const override;
        virtual QString copyright() const override;

        //////////
        //  emuone::util::IComponent
    public:
        virtual auto    version() const -> QVersionNumber override;
        virtual auto    settings() -> Settings * override;
        virtual auto    settings() const -> const Settings * override;
        virtual void    iniialize() override;
        virtual void    deiniialize() override;
    };
}

//  End of emuone-hades/Dynaload.hpp
