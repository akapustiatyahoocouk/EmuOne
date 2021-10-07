//
//  emuone-360/Ibm360Templates.cpp
//
//  TODO
//
//////////
#include "emuone-ibm360/API.hpp"

//////////
//  Ibm360H40VirtualMachineTemplate
IMPLEMENT_SINGLETON(Ibm360H40VirtualMachineTemplate)
Ibm360H40VirtualMachineTemplate::Ibm360H40VirtualMachineTemplate() {}
Ibm360H40VirtualMachineTemplate::~Ibm360H40VirtualMachineTemplate() {}

QString Ibm360H40VirtualMachineTemplate::getMnemonic() const
{
    return "IBM/360 H40";
}

QString Ibm360H40VirtualMachineTemplate::getDisplayName() const
{
    return "IBM/360 H40";
}

Architecture * Ibm360H40VirtualMachineTemplate::getArchitecture() const
{
    return Ibm360Architecture::getInstance();
}

void Ibm360H40VirtualMachineTemplate::populateVirtualAppliance(VirtualAppliance * virtualAppliance)
{
    virtualAppliance->addComponent(new Ibm360Storage("Main storage", MemorySize(MemorySize::Unit::KB, 256)));
}

//  End of emuone-360/Ibm360Templates.cpp
