//
//  emuone-hades/kernel/Kernel.cpp - emuone::hades::kernel::Kernel class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-hades/API.hpp"
using namespace emuone::hades::kernel;

//////////
//  Construction/destruction
Kernel::Kernel()
{
}

Kernel::~Kernel()
{
}

//////////
//  emuone::core::IComponent
auto Kernel::type() const -> emuone::core::IComponentType *
{
    return Type::instance();
}

QString Kernel::displayName() const
{
    return type()->displayName();
}

auto Kernel::createEditor(QWidget * parent) -> emuone::core::ComponentEditor *
{
    return new KernelEditor(parent, this);
}

void Kernel::saveConfiguration(QDomElement & element) const
{
    IComponent::saveConfiguration(element);
}

void Kernel::restoreConfiguration(const QDomElement & element)
{
    IComponent::restoreConfiguration(element);
}

//////////
//  emuone::core::IComponent (state control)
Kernel::State Kernel::state() const
{
    emuone::util::Lock _(guard);
    return _state;
}

void Kernel::connect()
{
}

void Kernel::disconnect() noexcept
{
}

void Kernel::initialize()
{
}

void Kernel::deinitialize() noexcept
{
}

void Kernel::start()
{
}

void Kernel::stop() noexcept
{
}

//////////
//  Operations (configuration)
QVersionNumber Kernel::version() const
{
    emuone::util::Lock _(guard);
    return _version;
}

void Kernel::setVersion(const QVersionNumber & version)
{
    emuone::util::Lock _(guard);

    if (state() != State::Constructed)
    {   //  OOPOS! Can't!
        throw emuone::core::InvalidComponentStateException();
    }
    if (version > QVersionNumber(0, 0, 0))
    {
        _version = version;
    }
}

SharedFolders Kernel::sharedFolders() const
{
    emuone::util::Lock _(guard);
    return _sharedFolders;
}

void Kernel::setSharedFolders(const SharedFolders & sharedFolders)
{
    emuone::util::Lock _(guard);

    if (state() != State::Constructed)
    {   //  OOPOS! Can't!
        throw emuone::core::InvalidComponentStateException();
    }
    QSet<QString> volumeNames;
    _sharedFolders.clear();
    for (auto sharedFolder : sharedFolders)
    {
        if (!volumeNames.contains(sharedFolder.volumeName()))
        {
            _sharedFolders.insert(sharedFolder);
            volumeNames.insert(sharedFolder.volumeName());
        }
    }
}

//////////
//  Kernel::Type
EMUONE_IMPLEMENT_SINGLETON(Kernel::Type)
Kernel::Type::Type() {}
Kernel::Type::~Type() {}

QString Kernel::Type::mnemonic() const
{
    return "emuone::hades::Kernel";
}

QString Kernel::Type::displayName() const
{
    return "HADES OS Kernel";
}

auto Kernel::Type::category() const -> emuone::core::IComponentCategory *
{
    return emuone::core::StandardComponentCategories::Software::instance();
}

bool Kernel::Type::isCompatibleWith(emuone::core::IArchitecture * architecture) const
{
    return architecture == Architecture::instance();
}

bool Kernel::Type::isCompatibleWith(emuone::core::IVirtualMachineType * virtualMachineType) const
{
    return virtualMachineType == emuone::core::StandardVirtualMachineTypes::VirtualMachine::instance();
}

bool Kernel::Type::isPersistable() const
{
    return false;
}

auto Kernel::Type::createComponent() -> Kernel *
{
    return new Kernel();
}

//  End of emuone-hades/kernel/Kernel.cpp
