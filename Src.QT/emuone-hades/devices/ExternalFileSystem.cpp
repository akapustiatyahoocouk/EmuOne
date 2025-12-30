//
//  emuone-hades/devices/ExternalFileSystem.cpp - emuone::hades::devices::ExternalFileSystem class implementation
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
using namespace emuone::hades::devices;

//////////
//  Construction/destruction
ExternalFileSystem::ExternalFileSystem()
    :   _volumeName("SYSTEM"),
        _hostPath(".")
{
}

ExternalFileSystem::~ExternalFileSystem()
{
}

//////////
//  emuone::core::IComponent
auto ExternalFileSystem::type() const -> emuone::core::IComponentType *
{
    return Type::instance();
}

QString ExternalFileSystem::displayName() const
{
    return "ExtFS: " + _volumeName + " -> " + _hostPath;
}

auto ExternalFileSystem::createEditor(QWidget * parent) -> emuone::core::ComponentEditor *
{
    return new ExternalFileSystemEditor(parent, this);
}

void ExternalFileSystem::saveConfiguration(QDomElement & element) const
{
    IDevice::saveConfiguration(element);

    element.setAttribute("VolumeName", _volumeName);
    element.setAttribute("HostPath", _hostPath);
}

void ExternalFileSystem::restoreConfiguration(const QDomElement & element)
{
    IDevice::restoreConfiguration(element);

    _volumeName = element.attribute("VolumeName", _volumeName);
    _hostPath = element.attribute("HostPath", _hostPath);
}

//////////
//  emuone::core::IComponent (state control)
ExternalFileSystem::State ExternalFileSystem::state() const
{
    emuone::util::Lock _(guard);
    return _state;
}

void ExternalFileSystem::connect()
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Constructed || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  There is nothing to connect to
    //  Perform state change
    _state = State::Connected;
}

void ExternalFileSystem::disconnect() noexcept
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  Nothing to do
        return;
    }
    //  There is nothing to disconnect
    //  Perform state change
    _state = State::Constructed;
}

void ExternalFileSystem::initialize()
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  There is nothing to intiialize
    //  Perform state change
    _state = State::Initialized;
}

void ExternalFileSystem::deinitialize() noexcept
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  Nothing to do
        return;
    }
    //  There is nothing to deinitialize
    //  Perform state change
    _state = State::Connected;
}

void ExternalFileSystem::start()
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  There is nothing to start
    //  Perform state change
    _state = State::Running;
}

void ExternalFileSystem::stop() noexcept
{
    emuone::util::Lock _(guard);

    //  Check preconditions
    if (_state != State::Running || !isBound())
    {   //  Nothing to do
        return;
    }
    //  There is nothing to stop
    //  Perform state change
    _state = State::Initialized;
}

//////////
//  Operations (configuration)

//////////
//  ExternalFileSystem::Type
EMUONE_IMPLEMENT_SINGLETON(ExternalFileSystem::Type)
ExternalFileSystem::Type::Type() {}
ExternalFileSystem::Type::~Type() {}

QString ExternalFileSystem::Type::mnemonic() const
{
    return "emuone::hades::devices::ExternalFileSystem";
}

QString ExternalFileSystem::Type::displayName() const
{
    return "External file system";
}

auto ExternalFileSystem::Type::category() const -> emuone::core::IComponentCategory *
{
    return emuone::core::StandardComponentCategories::Storage::instance();
}

bool ExternalFileSystem::Type::isCompatibleWith(emuone::core::IArchitecture * architecture) const
{
    return architecture == Architecture::instance();
}

bool ExternalFileSystem::Type::isCompatibleWith(emuone::core::IVirtualMachineType * virtualMachineType) const
{
    return virtualMachineType == emuone::core::StandardVirtualMachineTypes::VirtualMachine::instance();
}

bool ExternalFileSystem::Type::isPersistable() const
{
    return false;
}

auto ExternalFileSystem::Type::createComponent() -> ExternalFileSystem *
{
    return new ExternalFileSystem();
}

//////////
//  Operations (configuration)
bool ExternalFileSystem::isValidVolumeName(const QString & volumeName)
{   //  TODO implement properly
    if (volumeName.trimmed().length() != volumeName.length() ||
        volumeName.length() == 0)
    {   //  OOPS!
        return false;
    }
    return true;
}

bool ExternalFileSystem::isValidHostPath(const QString & hostPath)
{
    return QFileInfo(hostPath).isNativePath();
}

QString ExternalFileSystem::volumeName() const
{
    emuone::util::Lock _(guard);
    return _volumeName;
}

void ExternalFileSystem::setVolumeName(const QString & volumeName)
{
    emuone::util::Lock _(guard);
    Q_ASSERT(isValidHostPath(volumeName));  //  TODO throw instead

    _volumeName = volumeName;
}

QString ExternalFileSystem::hostPath() const
{
    emuone::util::Lock _(guard);
    return _hostPath;
}

void ExternalFileSystem::setHostPath(const QString & hostPath)
{
    emuone::util::Lock _(guard);
    Q_ASSERT(isValidHostPath(hostPath));    //  TODO throw instead
    _hostPath = hostPath;
}

//  End of emuone-hades/devices/ExternalFileSystem.cpp
