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
        Unit                getUnit() const { return _unit; }
        uint64_t            getNumberOfUnits() const { return _numberOfUnits; }

        //  Returns this memory size expressed in the specified units.
        uint64_t            toUnits(Unit unit) const;

        //  Returns this memory size expressed in bytes.
        uint64_t            toBytes() const;

        QString             toString() const;
        QString             toDisplayString() const;
        static MemorySize   fromString(const QString & s, const MemorySize & defaultValue);

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

//  End of emuone-core/MemorySize.hpp
