//
//  emuone-core/Component.cpp
//
//  The Component class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
Component::Component(const QString & name)
    :   _name(name)
{
    if (!isValidName(_name))
    {
        throw VirtualApplianceException("Invalid component name " + _name);
    }
}

Component::~Component()
{
    Q_ASSERT(_virtualAppliance == nullptr);
}

//////////
//  Operations
bool Component::isValidName(const QString & name)
{
    return name.length() > 0 && name.length() <= 255 &&
           name.trimmed().length() == name.length();
}

void Component::setName(const QString & name)
{
    if (!isValidName(name))
    {
        throw VirtualApplianceException("Invalid component name " + name);
    }

    _name = name;
}

//  End of emuone-core/Component.cpp
