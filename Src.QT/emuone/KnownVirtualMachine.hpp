//
//  emuone/KnownVirtualMachine.hpp - A VM known to have existed earlier
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
    /// \class KnownVirtualMachine API.hpp
    /// \brief A VM known to have existed earlier.
    class KnownVirtualMachine final
    {
        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs a known VM instance.
        /// \param name
        ///     The name of the VM.
        /// \param location
        ///     The location of the VM (full path
        ///     to its configuration file).
        KnownVirtualMachine(
                emuone::core::IArchitecture * architecture,
                emuone::core::IVirtualMachineType * type,
                const QString & name,
                const QString & location
            );

        //////////
        //  Operators
    public:
        bool        operator == (const KnownVirtualMachine & op2) const;
        bool        operator != (const KnownVirtualMachine & op2) const;

        //////////
        //  Operations
    public:
        auto        architecture(
                        ) const -> emuone::core::IArchitecture *
                    { return _architecture; }

        auto        type() const -> emuone::core::IVirtualMachineType *
                    { return _type; }

        /// \brief
        ///     Returns the name of the known VM.
        /// \return
        ///     The name of the known VM.
        QString     name() const { return _name; }

        /// \brief
        ///     Returns the location of the known VM.
        /// \return
        ///     The full path to the configuration file of the known VM.
        QString     location() const { return _location; }

        //////////
        //  Implementation
    private:
        emuone::core::IArchitecture *       _architecture;
        emuone::core::IVirtualMachineType * _type;
        QString     _name;
        QString     _location;
    };

    /// \brief
    ///     The ordered list of known VM records.
    using KnownVirtualMachines = QList<KnownVirtualMachine>;
}

//////////
//  Formatting and parsing
namespace emuone::util
{
    template <> QString toString<emuone::KnownVirtualMachine>(const emuone::KnownVirtualMachine & value);
    template <> QString toString<emuone::KnownVirtualMachines>(const emuone::KnownVirtualMachines & value);

    template <> emuone::KnownVirtualMachine fromString<emuone::KnownVirtualMachine>(const QString & s, qsizetype & scan);
    template <> emuone::KnownVirtualMachines fromString<emuone::KnownVirtualMachines>(const QString & s, qsizetype & scan);
}

//  End of emuone/KnownVirtualMachine.hpp
