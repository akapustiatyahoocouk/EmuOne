//
//  emuone-core/DataTypes.hpp
//
//  The basic EmuOne data types
//
//////////
#pragma once

namespace core
{
    //////////
    //  The "clock frequency" ADT
    class EMUONE_CORE_EXPORT ClockFrequency final
    {
        //////////
        //  Types
    public:
        enum class Unit
        {
            HZ,
            KHZ,
            MHZ,
            GHZ
        };

        //////////
        //  Construction/destruction/assignment
    public:
        ClockFrequency() : _unit(Unit::HZ), _numberOfUnits(0) {}
        ClockFrequency(Unit unit, uint64_t numberOfUnits) : _unit(unit), _numberOfUnits(numberOfUnits) {}

        //////////
        //  Operators
    public:
        bool                operator == (const ClockFrequency & op2) const { return this->toHz() == op2.toHz(); }
        bool                operator != (const ClockFrequency & op2) const { return this->toHz() != op2.toHz(); }
        bool                operator <  (const ClockFrequency & op2) const { return this->toHz() <  op2.toHz(); }
        bool                operator <= (const ClockFrequency & op2) const { return this->toHz() <= op2.toHz(); }
        bool                operator >  (const ClockFrequency & op2) const { return this->toHz() >  op2.toHz(); }
        bool                operator >= (const ClockFrequency & op2) const { return this->toHz() >= op2.toHz(); }

        ClockFrequency      operator / (int scale) const;
        int                 operator / (const ClockFrequency & subClockFrequency) const;

        //////////
        //  Operations
    public:
        Unit                    unit() const { return _unit; }
        uint64_t                numberOfUnits() const { return _numberOfUnits; }

        //  Returns this clock frequency expressed in the specified units.
        uint64_t                toUnits(Unit unit) const;

        //  Returns this clock frequency expressed in HZ.
        uint64_t                toHz() const;

        //  Returns a user-readable string representing this ClockFrequency.
        QString                 toDisplayString() const;

        //////////
        //  Implementation
    private:
        Unit                    _unit;
        uint64_t                _numberOfUnits;
    };

    //////////
    //  The "memory size" ADT
    class EMUONE_CORE_EXPORT MemorySize final
    {
        //////////
        //  Types
    public:
        enum class Unit
        {
            B,
            KB,
            MB,
            GB
        };

        //////////
        //  Construction/destruction/assignment
    public:
        MemorySize() : _unit(Unit::B), _numberOfUnits(0) {}
        MemorySize(Unit unit, uint64_t numberOfUnits) : _unit(unit), _numberOfUnits(numberOfUnits) {}

        //////////
        //  Operators
    public:
        bool                operator == (const MemorySize & op2) const { return this->toBytes() == op2.toBytes(); }
        bool                operator != (const MemorySize & op2) const { return this->toBytes() != op2.toBytes(); }
        bool                operator <  (const MemorySize & op2) const { return this->toBytes() <  op2.toBytes(); }
        bool                operator <= (const MemorySize & op2) const { return this->toBytes() <= op2.toBytes(); }
        bool                operator >  (const MemorySize & op2) const { return this->toBytes() >  op2.toBytes(); }
        bool                operator >= (const MemorySize & op2) const { return this->toBytes() >= op2.toBytes(); }

        //////////
        //  Operations
    public:
        Unit                unit() const { return _unit; }
        uint64_t            numberOfUnits() const { return _numberOfUnits; }

        //  Returns this memory size expressed in the specified units.
        uint64_t            toUnits(Unit unit) const;

        //  Returns this memory size expressed in bytes.
        uint64_t            toBytes() const;

        //  Returns a user-readable string representing this MemorySize.
        QString             toDisplayString() const;

        //////////
        //  Implementation
    private:
        Unit                _unit;
        uint64_t            _numberOfUnits;
    };

    //////////
    //  The "Duration" ADT
    class EMUONE_CORE_EXPORT Duration final
    {
        //////////
        //  Types
    public:
        enum class Unit
        {
            Seconds,
            Milliseconds,
            Microseconds,
            Nanoseconds
        };

        //////////
        //  Construction/destruction/assignment
    public:
        Duration() : _unit(Unit::Seconds), _numberOfUnits(0) {}
        Duration(Unit unit, uint64_t numberOfUnits) : _unit(unit), _numberOfUnits(numberOfUnits) {}

        //////////
        //  Operators
    public:
        bool                operator == (const Duration & op2) const { return this->toNs() == op2.toNs(); }
        bool                operator != (const Duration & op2) const { return this->toNs() != op2.toNs(); }
        bool                operator <  (const Duration & op2) const { return this->toNs() <  op2.toNs(); }
        bool                operator <= (const Duration & op2) const { return this->toNs() <= op2.toNs(); }
        bool                operator >  (const Duration & op2) const { return this->toNs() >  op2.toNs(); }
        bool                operator >= (const Duration & op2) const { return this->toNs() >= op2.toNs(); }

        //////////
        //  Operations
    public:
        Unit                    unit() const { return _unit; }
        uint64_t                numberOfUnits() const { return _numberOfUnits; }

        //  Returns this duration expressed in the specified units.
        uint64_t                toUnits(Unit unit) const;

        //  Returns this duration expressed in Nanoseconds.
        uint64_t                toNs() const;

        //  Returns a user-readable string representing this Duration.
        QString                 toDisplayString() const;

        //////////
        //  Implementation
    private:
        Unit                    _unit;
        uint64_t                _numberOfUnits;
    };

    //////////
    //  Helper functions
    EMUONE_CORE_EXPORT uint64_t getUnitValue(ClockFrequency::Unit unit);
    EMUONE_CORE_EXPORT uint64_t getUnitValue(MemorySize::Unit unit);
    EMUONE_CORE_EXPORT uint64_t getUnitValue(Duration::Unit unit);
}

//////////
//  Formatting/parsing
namespace util
{
    EMUONE_CORE_EXPORT QString toString(const core::ClockFrequency & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, qsizetype & scan, core::ClockFrequency & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, core::ClockFrequency & value);

    EMUONE_CORE_EXPORT QString toString(const core::MemorySize & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, qsizetype & scan, core::MemorySize & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, core::MemorySize & value);

    EMUONE_CORE_EXPORT QString toString(const core::Duration & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, qsizetype & scan, core::Duration & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, core::Duration & value);
}

//  End of emuone-core/DataTypes.hpp
