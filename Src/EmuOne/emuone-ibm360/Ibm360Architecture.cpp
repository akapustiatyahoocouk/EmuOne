//
//  emuone-360/Ibm360Architecture.cpp
//
//  Ibm360Architecture class implementation
//
//////////
#include "emuone-ibm360/API.hpp"

IMPLEMENT_SINGLETON(Ibm360Architecture)
Ibm360Architecture::Ibm360Architecture() {}
Ibm360Architecture::~Ibm360Architecture() {}

//////////
//  StockObject
QString Ibm360Architecture::getMnemonic() const
{
    return "IBM/360";
}

QString Ibm360Architecture::getDisplayName() const
{
    return "IBM/360";
}

QIcon Ibm360Architecture::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/Ibm360/Images/ArchitectureSmall.png");
    }
    return _smallIcon;
}

QIcon Ibm360Architecture::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/Ibm360/Images/ArchitectureLarge.png");
    }
    return _largeIcon;
}

//  End of emuone-360/Ibm360Architecture.cpp
