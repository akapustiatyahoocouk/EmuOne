//
//  emuone-core/ClockFrequency.hpp
//
//  The "clock frequency" ADT
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

        QString                 toString() const;
        static ClockFrequency   fromString(const QString & s, const ClockFrequency & defaultValue);
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
}

//  End of emuone-core/ClockFrequency.hpp
