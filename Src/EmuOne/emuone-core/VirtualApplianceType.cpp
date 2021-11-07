//
//  emuone-core/VirtualApplianceType.cpp
//
//  The VirtualApplianceType class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Registry
VirtualApplianceType * VirtualApplianceType::findByMnemonic(const QString & mnemonic)
{
    if (mnemonic == VirtualMachine::Type::instance()->mnemonic())
    {
        return VirtualMachine::Type::instance();
    }
    if (mnemonic == RemoteTerminal::Type::instance()->mnemonic())
    {
        return RemoteTerminal::Type::instance();
    }
    return nullptr;
}

//  End of emuone-core/VirtualApplianceType.cpp
