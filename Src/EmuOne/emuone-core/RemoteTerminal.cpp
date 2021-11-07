//
//  emuone-core/RemoteTerminal.cpp
//
//  The RemoteTerminal class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
RemoteTerminal::RemoteTerminal(const QString & name, const QString & location, Architecture * architecture, RemoteTerminalTemplate * remoteTerminalTemplate)
    :   VirtualAppliance(name, location, architecture, remoteTerminalTemplate)
{
}

RemoteTerminal::~RemoteTerminal()
{
}

//////////
//  VirtualAppliance
VirtualApplianceType * RemoteTerminal::type() const
{
    return Type::instance();
}

RemoteTerminalTemplate * RemoteTerminal::initialTemplate() const
{
    return dynamic_cast<RemoteTerminalTemplate*>(_template);
}

//////////
//  RemoteTerminal::Type
IMPLEMENT_SINGLETON(RemoteTerminal::Type)
RemoteTerminal::Type::Type() {}
RemoteTerminal::Type::~Type() {}

QString RemoteTerminal::Type::mnemonic() const
{
    return "RemoteTerminal";
}

QString RemoteTerminal::Type::displayName() const
{
    return "remote terminal";
}

QIcon RemoteTerminal::Type::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/RemoteTerminalSmall.png");
    }
    return _smallIcon;
}

QIcon RemoteTerminal::Type::largeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/RemoteTerminalLarge.png");
    }
    return _largeIcon;
}

RemoteTerminal * RemoteTerminal::Type::createVirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate)
{
    RemoteTerminalTemplate * remoteTerminalTemplate = dynamic_cast<RemoteTerminalTemplate*>(virtualApplianceTemplate);
    Q_ASSERT(remoteTerminalTemplate != nullptr);
    return new RemoteTerminal(name, location, architecture, remoteTerminalTemplate);
}

//  End of emuone-core/RemoteTerminal.cpp
