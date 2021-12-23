//
//  emuone-core/Duration.hpp
//
//  The "Duration" ADT
//
//////////
#pragma once

namespace core
{
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
    EMUONE_CORE_EXPORT uint64_t getUnitValue(Duration::Unit unit);
}

//////////
//  Formatting/parsing
namespace util
{
    EMUONE_CORE_EXPORT QString toString(const core::Duration & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, qsizetype & scan, core::Duration & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, core::Duration & value);
}

//  End of emuone-core/Duration.hpp
