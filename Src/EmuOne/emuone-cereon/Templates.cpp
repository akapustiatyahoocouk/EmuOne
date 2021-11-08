//
//  emuone-cereon/Templates.cpp
//
//  cereon template classes implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  CereonWorkstationMiniTemplate
IMPLEMENT_SINGLETON(CereonWorkstationMiniTemplate)
CereonWorkstationMiniTemplate::CereonWorkstationMiniTemplate() {}
CereonWorkstationMiniTemplate::~CereonWorkstationMiniTemplate() {}

QString CereonWorkstationMiniTemplate::mnemonic() const
{
    return "CereonWorkstationMini";
}

QString CereonWorkstationMiniTemplate::displayName() const
{
    return "Cereon workstation mini";
}

CereonWorkstationArchitecture * CereonWorkstationMiniTemplate::architecture() const
{
    return CereonWorkstationArchitecture::instance();
}

void CereonWorkstationMiniTemplate::populateVirtualAppliance(core::VirtualAppliance * /*virtualAppliance*/)
{
}

//  End of emuone-cereon/Templates.cpp
