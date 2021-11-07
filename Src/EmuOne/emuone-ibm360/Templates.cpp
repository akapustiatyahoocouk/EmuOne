//
//  emuone-360/Templates.cpp
//
//  TODO
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Ibm360H40VirtualMachineTemplate
IMPLEMENT_SINGLETON(H40VirtualMachineTemplate)
H40VirtualMachineTemplate::H40VirtualMachineTemplate() {}
H40VirtualMachineTemplate::~H40VirtualMachineTemplate() {}

QString H40VirtualMachineTemplate::mnemonic() const
{
    return "IBM/360 H40";
}

QString H40VirtualMachineTemplate::displayName() const
{
    return "IBM/360 H40";
}

Architecture * H40VirtualMachineTemplate::architecture() const
{
    return Architecture::instance();
}

void H40VirtualMachineTemplate::populateVirtualAppliance(core::VirtualAppliance * virtualAppliance)
{
    virtualAppliance->addComponent(new Processor("Processor", Features::All, core::ClockFrequency(core::ClockFrequency::Unit::KHZ, 1600)));
    virtualAppliance->addComponent(new Storage("Main storage", core::MemorySize(core::MemorySize::Unit::KB, 256)));
    virtualAppliance->addComponent(new Ibm2741("System console", 0x001));
}

//  End of emuone-360/Templates.cpp
