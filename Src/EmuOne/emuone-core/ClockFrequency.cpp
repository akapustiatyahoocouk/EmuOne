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

QString ClockFrequency::toString() const
{
    switch (_unit)
    {
        case ClockFrequency::Unit::HZ:
            return QString::number(_numberOfUnits) + "HZ";
        case ClockFrequency::Unit::KHZ:
            return QString::number(_numberOfUnits) + "KHZ";
        case ClockFrequency::Unit::MHZ:
            return QString::number(_numberOfUnits) + "MHZ";
        case ClockFrequency::Unit::GHZ:
            return QString::number(_numberOfUnits) + "GHZ";
        default:
            return "?";
    }
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

ClockFrequency ClockFrequency::fromString(const QString & s, const ClockFrequency & defaultValue)
{
    QString numberOfUnitsString = s;
    Unit unit = Unit::HZ;
    if (s.endsWith("GHZ"))
    {
        unit = Unit::GHZ;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 3);
    }
    else if (s.endsWith("MHZ"))
    {
        unit = Unit::MHZ;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 3);
    }
    else if (s.endsWith("KHZ"))
    {
        unit = Unit::KHZ;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 3);
    }
    else if (s.endsWith("HZ"))
    {
        unit = Unit::HZ;
        numberOfUnitsString = numberOfUnitsString.left(numberOfUnitsString.length() - 2);
    }

    uint64_t numberOfUnits = numberOfUnitsString.toUInt();
    if (numberOfUnits > 0)
    {
        return ClockFrequency(unit, numberOfUnits);
    }
    return defaultValue;
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

//  End of emuone-core/ClockFrequency.cpp
