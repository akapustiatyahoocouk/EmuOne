//
//  emuone-core/VirtualMachine.cpp
//
//  The VirtualMachine class implementation
//
//////////
#include "emuone-core/API.hpp"

//////////
//  Construction/destruction
VirtualMachine::VirtualMachine(const QString & name, const QString & location, Architecture * architecture, VirtualMachineTemplate * virtualMachineTemplate)
    :   VirtualAppliance(name, location, architecture, virtualMachineTemplate)
{
}

VirtualMachine::~VirtualMachine()
{
}

//////////
//  VirtualAppliance
VirtualApplianceType * VirtualMachine::getType() const
{
    return Type::getInstance();
}

VirtualMachineTemplate * VirtualMachine::getTemplate() const
{
    return dynamic_cast<VirtualMachineTemplate*>(_template);
}

//////////
//  VirtualMachine::Type
IMPLEMENT_SINGLETON(VirtualMachine::Type)
VirtualMachine::Type::Type() {}
VirtualMachine::Type::~Type() {}

QString VirtualMachine::Type::getMnemonic() const
{
    return "VirtualMachine";
}

QString VirtualMachine::Type::getDisplayName() const
{
    return "Virtual Machine";
}

QIcon VirtualMachine::Type::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/VirtualMachineSmall.png");
    }
    return _smallIcon;
}

QIcon VirtualMachine::Type::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/VirtualMachineLarge.png");
    }
    return _largeIcon;
}

VirtualMachine * VirtualMachine::Type::createVirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate)
{
    VirtualMachineTemplate * virtualMachineTemplate = dynamic_cast<VirtualMachineTemplate*>(virtualApplianceTemplate);
    Q_ASSERT(virtualMachineTemplate != nullptr);
    return new VirtualMachine(name, location, architecture, virtualMachineTemplate);
}

//  End of emuone-core/VirtualMachine.cpp
