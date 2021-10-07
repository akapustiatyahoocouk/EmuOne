//
//  emuone-core/Architecture.cpp
//
//  The Architecture class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

ArchitectureList Architecture::_registry;

//////////
//  Operations
//'void Architecture::addVirtualApplianceTemplate(VirtualApplianceTemplate * virtualApplianceTemplate);

bool Architecture::addVirtualMachineTemplate(VirtualMachineTemplate * virtualMachineTemplate)
{
    Q_ASSERT(virtualMachineTemplate != nullptr);

    VirtualMachineTemplate * registeredTemplate = findVirtualMachineTemplate(virtualMachineTemplate->getMnemonic());
    if (registeredTemplate == nullptr)
    {
        _virtualApplianceTemplates.append(virtualMachineTemplate);
        _virtualMachineTemplates.append(virtualMachineTemplate);
        return true;
    }
    return virtualMachineTemplate == registeredTemplate;
}

bool Architecture::addRemoteTerminalTemplate(RemoteTerminalTemplate * remoteTerminalTemplate)
{
    Q_ASSERT(remoteTerminalTemplate != nullptr);

    RemoteTerminalTemplate * registeredTemplate = findRemoteTerminalTemplate(remoteTerminalTemplate->getMnemonic());
    if (registeredTemplate == nullptr)
    {
        _virtualApplianceTemplates.append(remoteTerminalTemplate);
        _remoteTerminalTemplates.append(remoteTerminalTemplate);
        return true;
    }
    return remoteTerminalTemplate == registeredTemplate;
}

VirtualApplianceTemplate * Architecture::findVirtualApplianceTemplate(const QString & mnemonic) const
{
    for (VirtualApplianceTemplate * virtualApplianceTemplate : _virtualApplianceTemplates)
    {
        if (virtualApplianceTemplate->getMnemonic() == mnemonic)
        {
            return virtualApplianceTemplate;
        }
    }
    return nullptr;
}

VirtualMachineTemplate * Architecture::findVirtualMachineTemplate(const QString & mnemonic) const
{
    for (VirtualMachineTemplate * virtualMachineTemplate : _virtualMachineTemplates)
    {
        if (virtualMachineTemplate->getMnemonic() == mnemonic)
        {
            return virtualMachineTemplate;
        }
    }
    return nullptr;
}

RemoteTerminalTemplate * Architecture::findRemoteTerminalTemplate(const QString & mnemonic) const
{
    for (RemoteTerminalTemplate * remoteTerminalTemplate : _remoteTerminalTemplates)
    {
        if (remoteTerminalTemplate->getMnemonic() == mnemonic)
        {
            return remoteTerminalTemplate;
        }
    }
    return nullptr;
}

//////////
//  Registry
ArchitectureList Architecture::getAll()
{
    return _registry;
}

Architecture * Architecture::findByMnemonic(const QString & mnemonic)
{
    for (Architecture * architecture : _registry)
    {
        if (architecture->getMnemonic() == mnemonic)
        {
            return architecture;
        }
    }
    return nullptr;
}

bool Architecture::registerArchitecture(Architecture * architecture)
{
    Q_ASSERT(architecture != nullptr);

    Architecture * registeredArchitecture = findByMnemonic(architecture->getMnemonic());
    if (registeredArchitecture == nullptr)
    {
        _registry.append(architecture);
        return true;
    }
    return registeredArchitecture == architecture;
}

//  End of emuone-core/Architecture.cpp
