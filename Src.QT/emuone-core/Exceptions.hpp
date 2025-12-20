//
//  emuone-core/Exceptions.cpp - VM-related exceptions.
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

namespace emuone::core
{
    /// \class VirtualMachineException emuone-core/API.hpp
    /// \brief A common base class for all VM exceptions.
    class EMUONE_CORE_PUBLIC VirtualMachineException
        :   public emuone::util::Exception
    {
        using Self = VirtualMachineException;

        //////////
        //  Construction/destruction
    public:
        VirtualMachineException() = default;

        //////////
        //  QException
    public:
        Q_NORETURN
        void            raise() const override = 0;
        Self *          clone() const override = 0;
    };

    /// \class CustomVirtualMachineException emuone-core/API.hpp
    /// \brief A VM exception with a custom error message.
    class EMUONE_CORE_PUBLIC CustomVirtualMachineException
        :   public VirtualMachineException
    {
        using Self = CustomVirtualMachineException;

        //////////
        //  Construction/destruction
    public:
        explicit CustomVirtualMachineException(
                const QString & errorMessage
            );

        //////////
        //  QException
    public:
        Q_NORETURN
        void            raise() const override { throw *this; }
        Self *          clone() const override { return new Self(*this); }

        //////////
        //  emuone::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };
}

//  End of emuone-core/Exceptions.cpp
