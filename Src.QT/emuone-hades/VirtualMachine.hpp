//
//  emuone-hades/VirtualMachine.hpp - The HADES VM
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
    /// \class VirtualMachine emuone-hades/API.hpp
    /// \brief The HADES VM.
    class EMUONE_HADES_PUBLIC VirtualMachine final
        :   public emuone::core::VirtualMachine
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachine)

        friend class Architecture;

        //////////
        //  Construction/destruction
    private:
        /// \brief
        ///     Constructs an empty VM (with no components).
        /// \param name
        ///     The VM name.
        /// \param location
        ///     The VM location (a full path to the VM configuration file).
        /// \param type
        ///     The VM type; cannot be nullpyr.
        VirtualMachine(
                const QString & name,
                const QString & location,
                emuone::core::IVirtualMachineType * type
            );

        /// \brief
        ///     Constructs the VM from VM template.
        /// \param name
        ///     The VM name.
        /// \param location
        ///     The VM location (a full path to the VM configuration file).
        /// \param createdFrom
        ///     The template frpom which the VM was created; can be nullptr.
        VirtualMachine(
                const QString & name,
                const QString & location,
                emuone::core::IVirtualMachineTemplate * createdFrom
            );

        //////////
        //  emuone::core::VirtualMachine`  (configuration)
    public:
        virtual bool    isConfigurationValid() const override;
    };
}

//  End of emuone-hades/VirtualMachine.hpp
