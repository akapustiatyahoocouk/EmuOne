//
//  emuone-scp360/Device.cpp
//
//  The scp360::Device class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
scp360::Device::Device(Scp * scp, Id id, const QString & name, Flags flags, DeviceDriver * driver)
    :   Object(scp, id),
        _name(name),
        _flags(flags),
        _driver(driver)
{
    Q_ASSERT(_driver != nullptr);
}

scp360::Device::~Device()
{
}

//////////
//  Helper functions
EMUONE_SCP360_EXPORT scp360::Device::Flags scp360::operator & (Device::Flags op1, Device::Flags op2)
{
    return scp360::Device::Flags(uint16_t(op1) & uint16_t(op2));
}

EMUONE_SCP360_EXPORT scp360::Device::Flags scp360::operator | (Device::Flags op1, Device::Flags op2)
{
    return scp360::Device::Flags(uint16_t(op1) | uint16_t(op2));
}

EMUONE_SCP360_EXPORT scp360::Device::Flags & scp360::operator &= (Device::Flags & op1, Device::Flags op2)
{
    op1 = op1 & op2;
    return op1;
}

EMUONE_SCP360_EXPORT scp360::Device::Flags & scp360::operator |= (Device::Flags & op1, Device::Flags op2)
{
    op1 = op1 | op2;
    return op1;
}

//  End of emuone-scp360/Device.cpp
