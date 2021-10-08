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
    if (mnemonic == VirtualMachine::Type::getInstance()->mnemonic())
    {
        return VirtualMachine::Type::getInstance();
    }
    if (mnemonic == RemoteTerminal::Type::getInstance()->mnemonic())
    {
        return RemoteTerminal::Type::getInstance();
    }
    return nullptr;
}

//  End of emuone-core/VirtualApplianceType.cpp
