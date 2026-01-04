//
//  emuone-hades/Architecture.hpp - The HADES VM architecture
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
    /// \class Architecture emuone-hades/API.hpp
    /// \brief The HADES VM architecture.
    class EMUONE_HADES_PUBLIC Architecture final
        :   public virtual emuone::core::IArchitecture
    {
        EMUONE_DECLARE_SINGLETON(Architecture)

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QString mnemonic() const override;
        virtual QString displayName() const override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;

        //////////
        //  emuone::core::IArchitecture
    public:
        virtual auto    createVirtualMachine(
                                const QString & name,
                                const QString & location,
                                emuone::core::IVirtualMachineType * type
                            ) -> emuone::core::VirtualMachine * override;
    };
}

//  End of emuone-hades/Architecture.hpp
