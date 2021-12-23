//
//  emuone-core/ClockFrequency.cpp
//
//  The core::ClockFrequency class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Operators
ClockFrequency ClockFrequency::operator / (int scale) const
{
    Q_ASSERT(scale > 0);

    return ClockFrequency(ClockFrequency::Unit::HZ, this->_numberOfUnits / static_cast<unsigned>(scale));
}

int ClockFrequency::operator / (const ClockFrequency & subClockFrequency) const
{
    Q_ASSERT(subClockFrequency.toHz() > 0);

    return static_cast<int>(this->toHz() / subClockFrequency.toHz());
}

//////////
//  Operations
uint64_t ClockFrequency::toUnits(Unit unit) const
{
    return getUnitValue(_unit) * _numberOfUnits / getUnitValue(unit);
}

uint64_t ClockFrequency::toHz() const
{
    return getUnitValue(_unit) * _numberOfUnits;
}

QString ClockFrequency::toDisplayString() const
{
    switch (_unit)
    {
        case ClockFrequency::Unit::HZ:
            return QString::number(_numberOfUnits) + " HZ";
        case ClockFrequency::Unit::KHZ:
            return QString::number(_numberOfUnits) + " KHZ";
        case ClockFrequency::Unit::MHZ:
            return QString::number(_numberOfUnits) + " MHZ";
        case ClockFrequency::Unit::GHZ:
            return QString::number(_numberOfUnits) + " GHZ";
        default:
            return "?";
    }
}

//////////
//  Helper functions
uint64_t core::getUnitValue(ClockFrequency::Unit unit)
{
    switch (unit)
    {
        case ClockFrequency::Unit::HZ:
            return UINT64_C(1);
        case ClockFrequency::Unit::KHZ:
            return UINT64_C(1000);
        case ClockFrequency::Unit::MHZ:
            return UINT64_C(1000) * UINT64_C(1000);
        case ClockFrequency::Unit::GHZ:
            return UINT64_C(1000) * UINT64_C(1000) * UINT64_C(1000);
        default:
            return 1;
    }
}

//////////
//  Formatting/parsing
QString EMUONE_CORE_EXPORT util::toString(const core::ClockFrequency & value)
{
    switch (value.unit())
    {
        case ClockFrequency::Unit::HZ:
            return toString(value.numberOfUnits()) + "HZ";
        case ClockFrequency::Unit::KHZ:
            return toString(value.numberOfUnits()) + "KHZ";
        case ClockFrequency::Unit::MHZ:
            return toString(value.numberOfUnits()) + "MHZ";
        case ClockFrequency::Unit::GHZ:
            return toString(value.numberOfUnits()) + "GHZ";
        default:
            Q_ASSERT(false);
            return "?";
    }
}

bool EMUONE_CORE_EXPORT util::fromString(const QString & s, qsizetype & scan, core::ClockFrequency & value)
{
    qsizetype oldScan = scan;
    ClockFrequency oldValue = value;

    uint64_t numberOfUnits = 0;
    if (fromString(s, scan, numberOfUnits))
    {   //  ...now for the unit
        if (scan + 2 <= s.length() && s[scan].toLower() == 'h' && s[scan + 1].toLower() == 'z')
        {
            value = ClockFrequency(ClockFrequency::Unit::HZ, numberOfUnits);
            scan += 2;
            return true;
        }
        if (scan + 3 <= s.length() && s[scan].toLower() == 'k' && s[scan + 1].toLower() == 'h' && s[scan + 2].toLower() == 'z')
        {
            value = ClockFrequency(ClockFrequency::Unit::KHZ, numberOfUnits);
            scan += 3;
            return true;
        }
        if (scan + 3 <= s.length() && s[scan].toLower() == 'm' && s[scan + 1].toLower() == 'h' && s[scan + 2].toLower() == 'z')
        {
            value = ClockFrequency(ClockFrequency::Unit::MHZ, numberOfUnits);
            scan += 3;
            return true;
        }
        if (scan + 3 <= s.length() && s[scan].toLower() == 'g' && s[scan + 1].toLower() == 'h' && s[scan + 2].toLower() == 'z')
        {
            value = ClockFrequency(ClockFrequency::Unit::GHZ, numberOfUnits);
            scan += 3;
            return true;
        }
    }
    scan = oldScan;
    value = oldValue;
    return false;
}

bool EMUONE_CORE_EXPORT util::fromString(const QString & s, core::ClockFrequency & value)
{
    ClockFrequency oldValue = value;
    qsizetype scan = 0;
    if (fromString(s, scan, value) && scan == s.length())
    {
        return true;
    }
    //  OOPS! Restore "value" before returning
    value = oldValue;
    return false;
}

//  End of emuone-core/ClockFrequency.cpp
