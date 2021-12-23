//
//  emuone-core/MemorySize.hpp
//
//  The "memory size" ADT
//
//////////
#pragma once

namespace core
{
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
    //  Helper functions
    EMUONE_CORE_EXPORT uint64_t getUnitValue(MemorySize::Unit unit);
}

//////////
//  Formatting/parsing
namespace util
{
    EMUONE_CORE_EXPORT QString toString(const core::MemorySize & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, qsizetype & scan, core::MemorySize & value);
    EMUONE_CORE_EXPORT bool fromString(const QString & s, core::MemorySize & value);
}

//  End of emuone-core/MemorySize.hpp
