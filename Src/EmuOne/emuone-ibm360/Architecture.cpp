//
//  emuone-ibm360/Architecture.cpp
//
//  ibm360::Architecture class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

IMPLEMENT_SINGLETON(Architecture)
Architecture::Architecture() {}
Architecture::~Architecture() {}

//////////
//  StockObject
QString Architecture::getMnemonic() const
{
    return "IBM/360";
}

QString Architecture::getDisplayName() const
{
    return "IBM/360";
}

QIcon Architecture::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/Ibm360/Images/ArchitectureSmall.png");
    }
    return _smallIcon;
}

QIcon Architecture::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/Ibm360/Images/ArchitectureLarge.png");
    }
    return _largeIcon;
}

//  End of emuone-360/Architecture.cpp
