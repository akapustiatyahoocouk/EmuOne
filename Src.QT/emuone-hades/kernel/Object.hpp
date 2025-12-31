//
//  emuone-hades/kernel/Object.hpp - The HADES OS Kernel object
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

namespace emuone::hades::kernel
{
    /// \class Oid emuone-hades/API.hpp
    /// \brief The Lernel Object ID - unique.
    class EMUONE_HADES_PUBLIC Oid final
    {
        //////////
        //  Constants
    private:
        static const uint32_t _InvalidImpl = 0;
    public:
        static const Oid    Invalid;

        //////////
        //  Construction/destruction/assignment
    private:
        constexpr Oid(uint32_t impl) : _impl(impl) {}
    public:
        Oid();

        //  Default copy constructor, destructor and
        //  assignment are all OK.

        //////////
        //  Operatora
    public:
        bool            operator == (const Oid & op2) const;
        bool            operator != (const Oid & op2) const;
        bool            operator <  (const Oid & op2) const;
        bool            operator <= (const Oid & op2) const;
        bool            operator >  (const Oid & op2) const;
        bool            operator >= (const Oid & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether this OID is valid.
        /// \return
        ///     True if this OID is valid (can be used by so e Object),
        ///     false if not.
        bool            isValid() const { return _impl != _InvalidImpl; }

        //////////
        //  Implementation
    private:
        uint32_t    _impl;
    };

    /// \class Object emuone-hades/API.hpp
    /// \brief The HADES OS Kernel.object.
    class EMUONE_HADES_PUBLIC Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        //////////
        //  Construction/destruction
    public:
        Object(Kernel * kernel, const Oid & oid);
        virtual ~Object();

        //////////
        //  Properties
    public:
        /// \brief
        ///     The Kernel managing this Object.
        Kernel *const   kernel;

        /// \brief
        ///     The OID of this Object; unique per Kernel.
        const Oid       oid;

        //////////
        //  Implementation
    private:

        //  counts all pointers to thids object from kernel data structures
        uint32_t        _referenceCount = 0;
    };
}

//  End of emuone-hades/kernel/Object.hpp
