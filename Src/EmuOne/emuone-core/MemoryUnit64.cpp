//
//  emuone-core/MemoryUnit64.cpp
//
//  The core::MemoryUnit64 class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
MemoryUnit64::MemoryUnit64(const QString & name, uint64_t startAddress, const MemorySize & size)
    :   Component(name),
        _startAddress(startAddress),
        _size(size)
{
}

MemoryUnit64::~MemoryUnit64()
{
}

//////////
//  Component (serialisation)
void MemoryUnit64::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("StartAddress", ("0000000000000000" + QString::number(_startAddress, 16).toUpper()).right(16));
    configurationElement.setAttribute("Size", _size.toString());
}

void MemoryUnit64::deserialiseConfiguration(QDomElement & configurationElement)
{
    QString startAddressString = configurationElement.attribute("StartAddress", ("0000000000000000" + QString::number(_startAddress, 16).toUpper()).right(16));
    bool ok = false;
    uint64_t startAddress = startAddressString.toULongLong(&ok, 16);
    if (ok)
    {
        _startAddress = startAddress;
    }

    QString sizeString = configurationElement.attribute("Size", _size.toString());
    _size = core::MemorySize::fromString(sizeString, _size);
}

//  End of emuone-core/MemoryUnit64.cpp
