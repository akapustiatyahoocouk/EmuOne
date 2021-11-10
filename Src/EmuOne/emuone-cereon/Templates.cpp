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
    RamUnit * ram = new RamUnit("RAM", core::MemorySize(core::MemorySize::Unit::MB, 64), 0);
    virtualAppliance->addComponent(ram);

    RomUnit * rom = new RomUnit("ROM", core::MemorySize(core::MemorySize::Unit::MB, 1), UINT64_C(0xFFFFFFFFFFF00000), "./rom.bin");
    virtualAppliance->addComponent(rom);
    //  TODO create "rom bin" from resources ?

    Processor * processor = new Processor1P1B();
    virtualAppliance->addComponent(processor);
}

//  End of emuone-cereon/Templates.cpp
