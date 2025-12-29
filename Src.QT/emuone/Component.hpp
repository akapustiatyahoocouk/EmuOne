//
//  emuone/Component.hpp - emuone Component
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

namespace emuone
{
    /// \class Component emuon/API.hpp
    /// \brief the emuone component.
    class Component final
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

            //////////
            //  Constants
        public:
            /// \brief
            ///     The maximum number of recent VMs to keep.
            inline static const qsizetype MaxRecentVirtualMachines = 9;
            static_assert(MaxRecentVirtualMachines > 0 && MaxRecentVirtualMachines <= 9);

            //////////
            //  Properties
        public:
            /// \brief
            ///     The "normal" geometry of the main UI frame
            ///     (that is, when it is noe minimized or maximized).
            emuone::util::Setting<QByteArray>   mainFrameGeometry;

            /// \brief
            ///     True if the main UI frame is maximized, false if not.
            emuone::util::Setting<bool>    mainFrameMaximized;

            /// \brief
            ///     The recently opened VMs (youngest first, oldest last).
            emuone::util::Setting<KnownVirtualMachines> recentVirtualMachines;

            /// \brief
            ///     The location of the "current" VM, "" == no "current" VM.
            emuone::util::Setting<QString> currentVirtualMachineLocation;

            //////////
            //  Operations
        public:
            void        addRecentVirtualMachine(
                                const KnownVirtualMachine & kvm
                            );
             void       removeRecentVirtualMachine(
                                const KnownVirtualMachine & kvm
                            );
        };

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QString mnemonic() const override;
        virtual QString displayName() const override;

        //////////
        //  emuone::util::IComponent
    public:
        virtual auto    version() const -> QVersionNumber override;
        virtual QString copyright() const override;
        virtual auto    settings() -> Settings * override;
        virtual auto    settings() const -> const Settings * override;
        virtual void    iniialize() override;
        virtual void    deiniialize() override;
    };
}

//  End of emuone/Component.hpp
