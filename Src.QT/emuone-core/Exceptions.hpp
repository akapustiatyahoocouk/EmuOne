//
//  emuone-core/Exceptions.hpp - VM-related exceptions.
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

    /// \class VirtualMachineCorruptException emuone-core/API.hpp
    /// \brief Thrown when a VM is invalid or corrupt,
    class EMUONE_CORE_PUBLIC VirtualMachineCorruptException
        :   public VirtualMachineException
    {
        using Self = VirtualMachineCorruptException;

        //////////
        //  Construction/destruction
    public:
        explicit VirtualMachineCorruptException(
                const QString & location
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
        //  Operations
    public:
        QString         location() const { return _location; }

        //////////
        //  Implementation
    private:
        QString         _location;
    };

    /// \class InvalidVirtualMachineStateException emuone-core/API.hpp
    /// \brief Thrown when a VM service fails because VM is in an invalid state,
    class EMUONE_CORE_PUBLIC InvalidVirtualMachineStateException
        :   public VirtualMachineException
    {
        using Self = InvalidVirtualMachineStateException;

        //////////
        //  Construction/destruction
    public:
        InvalidVirtualMachineStateException();

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
    };

    /// \class InvalidComponentStateException emuone-core/API.hpp
    /// \brief Thrown when a VM service fails because component is in an invalid state,
    class EMUONE_CORE_PUBLIC InvalidComponentStateException
        :   public VirtualMachineException
    {
        using Self = InvalidComponentStateException;

        //////////
        //  Construction/destruction
    public:
        InvalidComponentStateException();

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
    };

    /// \class IncompatibleComponentException emuone-core/API.hpp
    /// \brief Thrown when adding an imcompatible component to a VM.
    class EMUONE_CORE_PUBLIC IncompatibleComponentException
        :   public VirtualMachineException
    {
        using Self = IncompatibleComponentException;

        //////////
        //  Construction/destruction
    public:
        IncompatibleComponentException(
                const QString & componentTypeMnemonic,
                const QString & componentTypeDisplayName
            );
        explicit IncompatibleComponentException(
                IComponent * component
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
        //  Operations
    public:
        QString         componentTypeMnemonic() const { return _componentTypeMnemonic; }
        QString         componentTypeDisplayName() const { return _componentTypeDisplayName; }

        //////////
        //  Implementation
    private:
        QString         _componentTypeMnemonic;
        QString         _componentTypeDisplayName;
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

//  End of emuone-core/Exceptions.hpp
