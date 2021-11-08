//
//  emuone-cereon/Architectures.cpp
//
//  cereon srchitecture classes implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  CereonArchitecture
QIcon CereonArchitecture::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/cereon/Images/ArchitectureSmall.png");
    }
    return _smallIcon;
}

QIcon CereonArchitecture::largeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/cereon/Images/ArchitectureLarge.png");
    }
    return _largeIcon;
}

//////////
//  CereonWorkstationArchitecture
IMPLEMENT_SINGLETON(CereonWorkstationArchitecture)
CereonWorkstationArchitecture::CereonWorkstationArchitecture() {}
CereonWorkstationArchitecture::~CereonWorkstationArchitecture() {}

QString CereonWorkstationArchitecture::mnemonic() const
{
    return "CereonWorkstation";
}

QString CereonWorkstationArchitecture::displayName() const
{
    return "Cereon workstation";
}

//  End of emuone-cereon/Architectures.cpp
