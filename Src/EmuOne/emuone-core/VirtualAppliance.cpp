//
//  emuone-core/VirtualAppliance.cpp
//
//  The VirtualAppliance class implementation
//
//////////
#include "emuone-core/API.hpp"

//////////
//  Operations
bool VirtualAppliance::isValidName(const QString & name)
{
    return name.length() > 0 && name.length() <= 255 &&
           name.trimmed().length() == name.length();
}

//  End of emuone-core/VirtualAppliance.cpp
