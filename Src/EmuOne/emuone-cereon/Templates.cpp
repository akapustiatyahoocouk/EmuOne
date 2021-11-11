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

void CereonWorkstationMiniTemplate::populateVirtualAppliance(core::VirtualAppliance * virtualAppliance)
{
    virtualAppliance->addComponent(new RamUnit("RAM", core::MemorySize(core::MemorySize::Unit::MB, 64), 0));

    virtualAppliance->addComponent(new RomUnit("ROM", core::MemorySize(core::MemorySize::Unit::MB, 1), UINT64_C(0xFFFFFFFFFFF00000), "./rom.bin"));
    //  TODO create "rom bin" from resources ?

    virtualAppliance->addComponent(new MemoryBus("Memory bus"));
    virtualAppliance->addComponent(new IoBus("I/O bus"));

    virtualAppliance->addComponent(new Processor1P1B());
}

//  End of emuone-cereon/Templates.cpp
