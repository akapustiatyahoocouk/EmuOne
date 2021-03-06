//
//  emuone-360/Ibm360Storage.cpp
//
//  ibm360::Storage class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Construction/destruction
Storage::Storage(const QString & name, const core::MemorySize & size)
    :   Component(name),
        _size(size)
{
}

Storage::~Storage()
{
}

//////////
//  Component
core::ComponentType * Storage::type() const
{
    return Storage::Type::instance();
}

core::ComponentEditor * Storage::createEditor(QWidget * parent)
{
    return new Ibm360StorageEditor(this, parent);
}

QString Storage::shortStatus() const
{
    return _size.toDisplayString();
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Storage::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Storage::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Locate the Processor - we need its Features for protection
    QList<Processor*> processors = this->virtualAppliance()->findComponentsByRole<Processor>();
    if (processors.isEmpty())
    {
        throw core::VirtualApplianceException("ibm360::Storage cannot locate ibm360::Processor");
    }
    if (processors.size() > 1)
    {
        throw core::VirtualApplianceException("ibm360::Storage located multiple ibm360::Processor");
    }
    _fetchProtection = has(processors[0]->features(), Features::FetchProtection);
    _storeProtection = has(processors[0]->features(), Features::StoreProtection);

    //  Done
    _state = State::Connected;
}

void Storage::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Storage::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Storage::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Storage::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }


    //  Done
    _state = State::Connected;
}

void Storage::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _fetchProtection = false;
    _storeProtection = false;

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void Storage::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("Size", util::toString(_size));
}

void Storage::deserialiseConfiguration(QDomElement & configurationElement)
{
    util::fromString(configurationElement.attribute("Size"), _size);
}

//////////
//  Operations
bool Storage::isValidSize(const core::MemorySize & size)
{
    return (size.toBytes() % 2048) == 0 && size.toBytes() >= (4 * 1024) && size.toBytes() <= (16 * 1024 * 1024);
}

bool Storage::setSize(const core::MemorySize & size)
{
    if (isValidSize(size))
    {
        _size = size;
        return true;
    }
    return false;
}

core::ComponentUi * Storage::createUi()
{
    return nullptr;
}

//////////
//  Ibm360Storage::Type
IMPLEMENT_SINGLETON(Storage::Type)
Storage::Type::Type() {}
Storage::Type::~Type() {}

QString Storage::Type::mnemonic() const
{
    return "ibm360::Storage";
}

QString Storage::Type::displayName() const
{
    return "IBM/360 main storage";
}

core::ComponentCategory * Storage::Type::category() const
{
    return core::StandardComponentCategories::Memory::instance();
}

bool Storage::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return architecture == Architecture::instance();
}

Storage * Storage::Type::createComponent()
{
    return new Storage("Main storage", core::MemorySize(core::MemorySize::Unit::KB, 128));
}

//  End of emuone-360/Storage.cpp
