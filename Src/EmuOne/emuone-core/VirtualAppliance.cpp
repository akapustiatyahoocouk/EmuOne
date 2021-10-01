//
//  emuone-core/VirtualAppliance.cpp
//
//  The VirtualAppliance class implementation
//
//////////
#include "emuone-core/API.hpp"

//////////
//  Constants
const QString VirtualAppliance::PreferredExtension = "emuone";

//////////
//  Construction/destruction
VirtualAppliance::VirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate)
    :   _name(name),
        _location(location),
        _architecture(architecture),
        _template(virtualApplianceTemplate)
{
    Q_ASSERT(_architecture != nullptr);
    Q_ASSERT(_template != nullptr);
}

VirtualAppliance::~VirtualAppliance()
{
}

//////////
//  Operations
bool VirtualAppliance::isValidName(const QString & name)
{
    return name.length() > 0 && name.length() <= 255 &&
           name.trimmed().length() == name.length();
}

void VirtualAppliance::setName(const QString & name)
{
    Q_ASSERT(isValidName(name));
    _name = name;
}

//  End of emuone-core/VirtualAppliance.cpp
