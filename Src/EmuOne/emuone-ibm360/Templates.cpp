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

QString H40VirtualMachineTemplate::getMnemonic() const
{
    return "IBM/360 H40";
}

QString H40VirtualMachineTemplate::getDisplayName() const
{
    return "IBM/360 H40";
}

Architecture * H40VirtualMachineTemplate::getArchitecture() const
{
    return Architecture::getInstance();
}

void H40VirtualMachineTemplate::populateVirtualAppliance(core::VirtualAppliance * virtualAppliance)
{
    virtualAppliance->addComponent(new Storage("Main storage", core::MemorySize(core::MemorySize::Unit::KB, 256)));
}

//  End of emuone-360/Templates.cpp
