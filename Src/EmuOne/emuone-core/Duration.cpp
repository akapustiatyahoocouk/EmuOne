//
//  emuone-core/Duration.cpp
//
//  The core::Duration class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Helper functions
EMUONE_CORE_EXPORT uint64_t core::getUnitValue(Duration::Unit unit)
{
    switch (unit)
    {
        case Duration::Unit::Seconds:
            return UINT64_C(1000000000);
        case Duration::Unit::Milliseconds:
            return UINT64_C(1000000);
        case Duration::Unit::Microseconds:
            return UINT64_C(1000);
        case Duration::Unit::Nanoseconds:
            return UINT64_C(1);
        default:
            Q_ASSERT(false);
            return UINT64_C(1);
    }
}

//////////
//  Formatting/parsing
QString EMUONE_CORE_EXPORT util::toString(const core::Duration & value)
{
    switch (value.unit())
    {
        case Duration::Unit::Seconds:
            return toString(value.numberOfUnits()) + "s";
        case Duration::Unit::Milliseconds:
            return toString(value.numberOfUnits()) + "ms";
        case Duration::Unit::Microseconds:
            return toString(value.numberOfUnits()) + "us";
        case Duration::Unit::Nanoseconds:
            return toString(value.numberOfUnits()) + "ns";
        default:
            Q_ASSERT(false);
            return "?";
    }
}

bool EMUONE_CORE_EXPORT util::fromString(const QString & s, qsizetype & scan, core::Duration & value)
{
    qsizetype oldScan = scan;
    Duration oldValue = value;

    uint64_t numberOfUnits = 0;
    if (fromString(s, scan, numberOfUnits))
    {   //  ...now for the unit
        if (scan + 1 <= s.length() && s[scan].toLower() == 's')
        {
            value = Duration(Duration::Unit::Seconds, numberOfUnits);
            scan++;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'm' && s[scan + 1].toLower() == 's')
        {
            value = Duration(Duration::Unit::Milliseconds, numberOfUnits);
            scan += 2;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'u' && s[scan + 1].toLower() == 's')
        {
            value = Duration(Duration::Unit::Microseconds, numberOfUnits);
            scan += 2;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'n' && s[scan + 1].toLower() == 's')
        {
            value = Duration(Duration::Unit::Nanoseconds, numberOfUnits);
            scan += 2;
            return true;
        }
    }
    scan = oldScan;
    value = oldValue;
    return false;
}

bool EMUONE_CORE_EXPORT util::fromString(const QString & s, core::Duration & value)
{
    Duration oldValue = value;
    qsizetype scan = 0;
    if (fromString(s, scan, value) && scan == s.length())
    {
        return true;
    }
    //  OOPS! Restore "value" before returning
    value = oldValue;
    return false;
}

//  End of emuone-core/Duration.cpp
