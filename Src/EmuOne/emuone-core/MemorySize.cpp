//
//  emuone-core/MemorySize.cpp
//
//  The MemorySize class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Operations
uint64_t MemorySize::toUnits(Unit unit) const
{
    return getUnitValue(_unit) * _numberOfUnits / getUnitValue(unit);
}

uint64_t MemorySize::toBytes() const
{
    return getUnitValue(_unit) * _numberOfUnits;
}

QString MemorySize::toDisplayString() const
{
    switch (_unit)
    {
        case MemorySize::Unit::B:
            return QString::number(_numberOfUnits) + " B";
        case MemorySize::Unit::KB:
            return QString::number(_numberOfUnits) + " KB";
        case MemorySize::Unit::MB:
            return QString::number(_numberOfUnits) + " MB";
        case MemorySize::Unit::GB:
            return QString::number(_numberOfUnits) + " GB";
        default:
            return "?";
    }
}

//////////
//  Helper functions
uint64_t core::getUnitValue(MemorySize::Unit unit)
{
    switch (unit)
    {
        case MemorySize::Unit::B:
            return UINT64_C(1);
        case MemorySize::Unit::KB:
            return UINT64_C(1024);
        case MemorySize::Unit::MB:
            return UINT64_C(1024) * UINT64_C(1024);
        case MemorySize::Unit::GB:
            return UINT64_C(1024) * UINT64_C(1024) * UINT64_C(1024);
        default:
            return 1;
    }
}

//////////
//  Formatting/parsing
EMUONE_CORE_EXPORT QString util::toString(const core::MemorySize & value)
{
    switch (value.unit())
    {
        case MemorySize::Unit::B:
            return QString::number(value.numberOfUnits()) + "B";
        case MemorySize::Unit::KB:
            return QString::number(value.numberOfUnits()) + "KB";
        case MemorySize::Unit::MB:
            return QString::number(value.numberOfUnits()) + "MB";
        case MemorySize::Unit::GB:
            return QString::number(value.numberOfUnits()) + "GB";
        default:
            return "?";
    }
}

bool EMUONE_CORE_EXPORT util::fromString(const QString & s, qsizetype & scan, core::MemorySize & value)
{
    qsizetype oldScan = scan;
    MemorySize oldValue = value;

    uint64_t numberOfUnits = 0;
    if (fromString(s, scan, numberOfUnits))
    {   //  ...now for the unit
        if (scan + 1 <= s.length() && s[scan].toLower() == 'b')
        {
            value = MemorySize(MemorySize::Unit::B, numberOfUnits);
            scan++;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'k' && s[scan + 1].toLower() == 'b')
        {
            value = MemorySize(MemorySize::Unit::KB, numberOfUnits);
            scan += 2;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'm' && s[scan + 1].toLower() == 'b')
        {
            value = MemorySize(MemorySize::Unit::MB, numberOfUnits);
            scan += 2;
            return true;
        }
        if (scan + 2 <= s.length() && s[scan].toLower() == 'g' && s[scan + 1].toLower() == 'b')
        {
            value = MemorySize(MemorySize::Unit::GB, numberOfUnits);
            scan += 2;
            return true;
        }
    }
    scan = oldScan;
    value = oldValue;
    return false;
}

EMUONE_CORE_EXPORT bool util::fromString(const QString & s, core::MemorySize & value)
{
    MemorySize oldValue = value;
    qsizetype scan = 0;
    if (fromString(s, scan, value) && scan == s.length())
    {
        return true;
    }
    //  OOPS! Restore "value" before returning
    value = oldValue;
    return false;
}

//  End of emuone-core/MemorySize.cpp
