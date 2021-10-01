//
//  emuone-360/Ibm360H40VirtualMachineTemplate.cpp
//
//  TODO
//
//////////
#include "emuone-ibm360/API.hpp"

IMPLEMENT_SINGLETON(Ibm360H40VirtualMachineTemplate)
Ibm360H40VirtualMachineTemplate::Ibm360H40VirtualMachineTemplate() {}
Ibm360H40VirtualMachineTemplate::~Ibm360H40VirtualMachineTemplate() {}

//////////
//  StockObject
QString Ibm360H40VirtualMachineTemplate::getMnemonic() const
{
    return "IBM/360 H40";
}

QString Ibm360H40VirtualMachineTemplate::getDisplayName() const
{
    return "IBM/360 H40";
}

//////////
//  VirtualApplianceTemplate
Architecture * Ibm360H40VirtualMachineTemplate::getArchitecture() const
{
    return Ibm360Architecture::getInstance();
}

//  End of emuone-360/Ibm360H40VirtualMachineTemplate.cpp
