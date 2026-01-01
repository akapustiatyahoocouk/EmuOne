//
//  emuone-hades/kernel/DataTypes.hpp - Kernel data types
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
    enum class StatusCode : uint32_t
    {
        Success         = 0x00000000,
        InvalidArgument = 0x00000001,
        AlreadyExists   = 0x00000002,
        DoesNotExist    = 0x00000003,
        UnknownError    = 0x7FFFFFFF
    };

    /// \class Oid emuone-hades/API.hpp
    /// \brief The Lernel Object ID - unique.
    class EMUONE_HADES_PUBLIC Oid final
    {
        friend class Kernel;

        //////////
        //  Constants
    private:
        static const uint32_t _InvalidImpl = 0;
    public:
        //  Special OIDs
        static const Oid    Invalid;

        //  Fixed OIDs
        static const Oid    MinFixedOid;    //  inclusive
        static const Oid    MaxFixedOid;    //  inclusive

        //  Random OIDs
        static const Oid    MinRandomOid;   //  inclusive
        static const Oid    MaxRandomOid;   //  inclusive

        //////////
        //  Construction/destruction/assignment
    private:
        constexpr Oid(uint32_t impl) : _impl(impl) {}
    public:
        /// \brief
        ///     Constructs an invalid OID.
        Oid() : _impl(_InvalidImpl) {}

        //  Default copy constructor, destructor and
        //  assignment are all OK.

        //////////
        //  Operatora
    public:
        bool            operator == (const Oid & op2) const { return _impl == op2._impl; }
        bool            operator != (const Oid & op2) const { return _impl != op2._impl; }
        bool            operator <  (const Oid & op2) const { return _impl <  op2._impl; }
        bool            operator <= (const Oid & op2) const { return _impl <= op2._impl; }
        bool            operator >  (const Oid & op2) const { return _impl >  op2._impl; }
        bool            operator >= (const Oid & op2) const { return _impl >= op2._impl; }

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

    /// \brief
    ///     Standard device type IDs.
    enum DeviceTypeId : uint16_t
    {
        Amd64Processor      = 0x0100,
        Amd64ProcessorCore  = 0x0101,
#if defined(Q_PROCESSOR_X86_64)
        HostProcessor       = Amd64Processor,
        HostProcessorCore   = Amd64ProcessorCore,
#else
    #error Unsupported processor architecture
#endif
    };
}

//  End of emuone-hades/kernel/DataTypes.hpp
