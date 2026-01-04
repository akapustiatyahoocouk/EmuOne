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
    return type()->displayName() +
           " " +
           emuone::util::toString(_version);
}

auto Kernel::createEditor(QWidget * parent) -> emuone::core::ComponentEditor *
{
    return new KernelEditor(parent, this);
}

bool Kernel::isConfigurationValid() const
{   //  Nothing here
    return true;
}

void Kernel::saveConfiguration(QDomElement & element) const
{
    IComponent::saveConfiguration(element);

    element.setAttribute(
        "Version",
        emuone::util::toString(_version));
}

void Kernel::restoreConfiguration(const QDomElement & element)
{
    IComponent::restoreConfiguration(element);

    _version = emuone::util::fromString(
        element.attribute("Version"),
        _version);
}

//////////
//  emuone::core::IComponent (state control)
Kernel::State Kernel::state() const
{
    emuone::util::Lock _(stateGuard);
    return _state;
}

void Kernel::connect()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Constructed || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  TODO connect
    //  Perform state change
    _state = State::Connected;
}

void Kernel::disconnect() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  Nothing to do
        return;
    }
    //  TODO disconnect
    //  Perform state change
    _state = State::Constructed;
}

void Kernel::initialize()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  TODO initialize
    //  Perform state change
    _state = State::Initialized;
}

void Kernel::deinitialize() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  Nothing to do
        return;
    }
    //  TODO deinitialize
    //  Perform state change
    _state = State::Connected;
}

void Kernel::start()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }

    //  Kernel in non-persistable, so we must always
    //  perform a cold start
    Q_ASSERT(_objects.isEmpty());
    Q_ASSERT(_identities.isEmpty());
    Q_ASSERT(_systemIdentity == nullptr);
    Q_ASSERT(_deviceTypes.isEmpty());
    Q_ASSERT(_processors.isEmpty());
    Q_ASSERT(_executors.isEmpty());
    Q_ASSERT(_executionEnvironments.isEmpty());
    //  TODO other secondary caches

    //  To start the Kernel, we need several things...
    {
        emuone::util::Lock _1(kernelGuard);

        //  1.  To create a System identity,
        SystemIdentity * systemIdentity = nullptr;
        createSystemIdentity(systemIdentity);
        Q_ASSERT(systemIdentity != nullptr);

        //  2.  To create native Processor and ProcessorCores
        //      along with DeviceTypes for both
        DeviceType * processorDeviceType,
                   * processorCoreDeviceType;
        createDeviceType(
            systemIdentity,
            DeviceTypeId::HostProcessor,
            defaultDeviceTypeName(DeviceTypeId::HostProcessor),
            processorDeviceType);
        Q_ASSERT(processorDeviceType != nullptr);

        createDeviceType(
            systemIdentity,
            DeviceTypeId::HostProcessorCore,
            defaultDeviceTypeName(DeviceTypeId::HostProcessorCore),
            processorCoreDeviceType);
        Q_ASSERT(processorCoreDeviceType != nullptr);

        Processor * processor = nullptr;
        createProcessor(
            systemIdentity,
            processorDeviceType,
            ProcessorId(0),
            processor);
        Q_ASSERT(processor != nullptr);

        int numHostCores = QThread::idealThreadCount();
        numHostCores = std::max(1, std::min(255, numHostCores));
        ProcessorCores processorCores;
        for (int i = 0; i < numHostCores; i++)
        {
            ProcessorCore * processorCore = nullptr;
            createProcessorCore(
                systemIdentity,
                processorCoreDeviceType,
                processor,
                CoreId(static_cast<uint8_t>(i)),
                processorCore);
            Q_ASSERT(processorCore != nullptr);
            processorCores.insert(processorCore);
        }

        //  3.  To create the default Executor(s) and
        //      ExecutionEnvironment.
        NativeExecutors nativeExecutors;
        for (auto processorCore : processorCores)
        {
            PNativeExecutor nativeExecutor = nullptr;
            createNativeExecutor(
                systemIdentity,
                processorCore,
                nativeExecutor);
            Q_ASSERT(nativeExecutor != nullptr);
            nativeExecutors.insert(nativeExecutor);
        }
        Q_ASSERT(!nativeExecutors.isEmpty());

        PNativeExecutionEnvironment nativeExecutionEnvironment = nullptr;
        createNativeExecutionEnvironment(
            systemIdentity,
            "SYSTEM",
            nativeExecutors,
            nativeExecutionEnvironment);
        Q_ASSERT(nativeExecutionEnvironment != nullptr);

        //  4.  To create an init process with a single
        //      native thread (both owned by System identity),
        //  TODO

        //  5.  And start that init thread
        //  TODO
    }

    //  Perform state change
    _state = State::Running;
}

void Kernel::stop() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Running || !isBound())
    {   //  Nothing to do
        return;
    }

    //  Terminate all native threads, politrly if
    //  possible, forcibly otherwise
    //  TODO

    //  Destroy all kernel objects and clear the
    //  primary and secondary caches
    {
        emuone::util::Lock _1(kernelGuard);
        _shutdownInProgress = true; //  we're killing EVERYTHING!
        for (Object * object : _objects.values())   //  shallow clone
        {
            delete object;
        }
        Q_ASSERT(_objects.isEmpty());
        Q_ASSERT(_identities.isEmpty());
        Q_ASSERT(_systemIdentity == nullptr);
        Q_ASSERT(_deviceTypes.isEmpty());
        Q_ASSERT(_processors.isEmpty());
        Q_ASSERT(_executors.isEmpty());
        Q_ASSERT(_executionEnvironments.isEmpty());
        //  TODO other secondary caches
        _shutdownInProgress = false;    //  we're done shutting down the Kernel
    }

    //  Perform state change
    _state = State::Initialized;
}

//////////
//  Operations (configuration)
QVersionNumber Kernel::version() const
{
    emuone::util::Lock _(stateGuard);
    return _version;
}

void Kernel::setVersion(const QVersionNumber & version)
{
    emuone::util::Lock _(stateGuard);

    if (state() != State::Constructed)
    {   //  OOPOS! Can't!
        throw emuone::core::InvalidComponentStateException();
    }
    if (version > QVersionNumber(0, 0, 0))
    {
        _version = version;
    }
}

//////////
//  Kernel::Type
EMUONE_IMPLEMENT_SINGLETON(Kernel::Type)
Kernel::Type::Type() {}
Kernel::Type::~Type() {}

QString Kernel::Type::mnemonic() const
{
    return "emuone::hades::kernel::Kernel";
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
