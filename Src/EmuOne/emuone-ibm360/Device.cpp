//
//  emuone-360/Device.cpp
//
//  ibm360::Device class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Construction/destruction
Device::Device(const QString & name, uint16_t address)
    :   core::Component(name),
        _address(address)
{
}

Device::~Device()
{
}

//////////
//  Component (serialisation)
void Device::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("Address", ("000" + QString::number(_address, 16)).right(3));
}

void Device::deserialiseConfiguration(QDomElement & configurationElement)
{
    bool addressOk = false;
    uint16_t address = (uint16_t)configurationElement.attribute("Address").toInt(&addressOk, 16);
    if (addressOk && isValidAddress(address))
    {
        _address = address;
    }
}

//////////
//  Operations
bool Device::isValidAddress(uint16_t address)
{
    return address <= 0x6FF;
}

void Device::setAddress(uint16_t address)
{
    if (!isValidAddress(address))
    {
        throw core::VirtualApplianceException("Invalid device address " +
                                              ("000" + QString::number(address, 16)).right(3));
    }
    _address = address;
}

//  End of emuone-360/Device.cpp
