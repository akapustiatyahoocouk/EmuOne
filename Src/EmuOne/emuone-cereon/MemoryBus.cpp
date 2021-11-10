//
//  emuone-cereon/MemoryBus.cpp
//
//  The cereon::MemoryBus class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
MemoryBus::MemoryBus(const QString & name)
    :  core::Component(name)
{
}

MemoryBus::~MemoryBus()
{
    delete [] _memoryUnitMappings;  //  ...just in case
}

//////////
//  core::Component
core::ComponentType * MemoryBus::type() const
{
    return Type::instance();
}

core::ComponentEditor * MemoryBus::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString MemoryBus::shortStatus() const
{
    return this->type()->displayName();
}

core::ComponentUi * MemoryBus::createUi()
{
    return nullptr;
}

//////////
//  core::Component (state control) - all thread-safe
MemoryBus::State MemoryBus::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void MemoryBus::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Locate all memory units
    BoundMemoryUnitList memoryUnits;
    for (IMemoryBusClient * memoryBusClient : virtualAppliance()->findComponents<IMemoryBusClient>())
    {
        memoryUnits.append(memoryBusClient->memoryUnits());
    }
    for (IMemoryBusClient * memoryBusClient : virtualAppliance()->findAdaptors<IMemoryBusClient>())
    {
        memoryUnits.append(memoryBusClient->memoryUnits());
    }
    //  Validate them
    //  Attach

    //  Done
    _state = State::Connected;
}

void MemoryBus::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void MemoryBus::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void MemoryBus::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void MemoryBus::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void MemoryBus::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Drop connections to memory units
    _numMemoryUnitMappings = 0;
    delete _memoryUnitMappings;
    _memoryUnitMappings = nullptr;

    //  Done
    _state = State::Constructed;
}

//////////
//  core::Component (serialisation)
void MemoryBus::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void MemoryBus::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//////////
//  MemoryBus::Type
IMPLEMENT_SINGLETON(MemoryBus::Type)
MemoryBus::Type::Type() {}
MemoryBus::Type::~Type() {}

QString MemoryBus::Type::mnemonic() const
{
    return "Cereon-MemoryBus";
}

QString MemoryBus::Type::displayName() const
{
    return "Cereon Memory Bus";
}

core::ComponentCategory * MemoryBus::Type::category() const
{
    return core::StandardComponentCategories::Motherboard::instance();
}

bool MemoryBus::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return dynamic_cast<CereonArchitecture*>(architecture) != nullptr;
}


core::Component * MemoryBus::Type::createComponent()
{
    return new MemoryBus("Memory bus");
}

//  End of emuone-cereon/MemoryBus.cpp
