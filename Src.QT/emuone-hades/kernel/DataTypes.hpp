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

    /// \class Id emuone-hades/API.hpp
    /// \brief A generic ID of something within ght kernel.
    template <class T>
    class Id
    {
        static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");
        static_assert(std::is_integral <T>::value, "T must be an integer type");

        friend class Kernel;

        //////////
        //  Constants
    private:
        static const T  _InvalidImpl = ~T(0);

        //////////
        //  Construction/destruction/assignment
    public:
        explicit constexpr Id(T impl) : _impl(impl) {}
    public:
        /// \brief
        ///     Constructs an invalid OID.
        Id() : _impl(_InvalidImpl) {}

        //  Default copy constructor, destructor and
        //  assignment are all OK.

        //////////
        //  Operators
    public:
        bool            operator == (const Id<T> & op2) const { return _impl == op2._impl; }
        bool            operator != (const Id<T> & op2) const { return _impl != op2._impl; }
        bool            operator <  (const Id<T> & op2) const { return _impl <  op2._impl; }
        bool            operator <= (const Id<T> & op2) const { return _impl <= op2._impl; }
        bool            operator >  (const Id<T> & op2) const { return _impl >  op2._impl; }
        bool            operator >= (const Id<T> & op2) const { return _impl >= op2._impl; }
        explicit        operator T() const { return _impl; }

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
        T               _impl;
    };

    /// \brief The Kernel Object ID - unique.
    class EMUONE_HADES_PUBLIC Oid final : public Id<uint32_t>
    {
        //////////
        //  Constants
    public:
        //  Fixed OID range
        static const Oid    MinFixedOid;    //  inclusive
        static const Oid    MaxFixedOid;    //  inclusive

        //  Random OIDs
        static const Oid    MinRandomOid;   //  inclusive
        static const Oid    MaxRandomOid;   //  inclusive

        //////////
        //  Construction/destruction
    public:
        Oid() = default;
        explicit Oid(uint32_t impl) : Id<uint32_t>(impl) {}
    };

    /// \brief An ID of the Device, unique per Kernel.
    class EMUONE_HADES_PUBLIC DeviceTypeId final : public Id<uint16_t>
    {
        //////////
        //  Constants
    public:
        static const DeviceTypeId   Amd64Processor;
        static const DeviceTypeId   Amd64ProcessorCore;
        static const DeviceTypeId   HostProcessor;
        static const DeviceTypeId   HostProcessorCore;

        //////////
        //  Construction/destruction
    public:
        DeviceTypeId() = default;
        explicit DeviceTypeId(uint16_t impl) : Id<uint16_t>(impl) {}
    };

    /// \brief An ID of the Device, unique per DeviceType.
    using DeviceId = Id<uint16_t>;

    /// \brief //  An ID of a Processor, unique per Kernel.
    using ProcessorId = Id<uint8_t>;

    /// \brief An ID of a ProcessorCore, unique per Processor.
    using CoreId = Id<uint8_t>;
}

//  End of emuone-hades/kernel/DataTypes.hpp
