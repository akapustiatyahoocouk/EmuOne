//
//  emuone-core/MemorySize.cpp
//
//  The MemorySize class implementation
//
//////////
#include "emuone-core/API.hpp"

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

QString MemorySize::toString() const
{
    switch (_unit)
    {
        case MemorySize::Unit::B:
            return QString::number(_numberOfUnits) + "B";
        case MemorySize::Unit::KB:
            return QString::number(_numberOfUnits) + "KB";
        case MemorySize::Unit::MB:
            return QString::number(_numberOfUnits) + "MB";
        case MemorySize::Unit::GB:
            return QString::number(_numberOfUnits) + "GB";
        default:
            return "?";
    }
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

MemorySize MemorySize::fromString(const QString & s, const MemorySize & defaultValue)
{
    QString numberOfUnitsString = s;
    Unit unit = Unit::B;
    if (s.endsWith("GB"))
    {
        unit = Unit::GB;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 2);
    }
    else if (s.endsWith("MB"))
    {
        unit = Unit::MB;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 2);
    }
    else if (s.endsWith("KB"))
    {
        unit = Unit::KB;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 2);
    }
    else if (s.endsWith("B"))
    {
        unit = Unit::B;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 1);
    }

    uint64_t numberOfUnits = numberOfUnitsString.toUInt();
    if (numberOfUnits > 0)
    {
        return MemorySize(unit, numberOfUnits);
    }
    return defaultValue;
}

//////////
//  Helper functions
uint64_t getUnitValue(MemorySize::Unit unit)
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

//  End of emuone-core/MemorySize.cpp
