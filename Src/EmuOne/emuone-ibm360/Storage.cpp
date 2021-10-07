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
core::ComponentType * Storage::getType() const
{
    return Storage::Type::getInstance();
}

ComponentEditor * Storage::createEditor(QWidget * parent)
{
    return new Ibm360StorageEditor(this, parent);
}

QString Storage::getShortStatus() const
{
    return _size.toDisplayString();
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Storage::getState() const
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

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void Storage::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("Size", _size.toString());
}

void Storage::deserialiseConfiguration(QDomElement & configurationElement)
{
    QString sizeString = configurationElement.attribute("Size", _size.toString());
    _size = core::MemorySize::fromString(sizeString, _size);
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

//////////
//  Ibm360Storage::Type
IMPLEMENT_SINGLETON(Storage::Type)
Storage::Type::Type() {}
Storage::Type::~Type() {}

QString Storage::Type::getMnemonic() const
{
    return "ibm360::Storage";
}

QString Storage::Type::getDisplayName() const
{
    return "IBM/360 main storage";
}

core::ComponentCategory * Storage::Type::getCategory() const
{
    return core::StandardComponentCategories::Memory::getInstance();
}

bool Storage::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Storage * Storage::Type::createComponent()
{
    return new Storage("Main storage", core::MemorySize(core::MemorySize::Unit::KB, 128));
}

//  End of emuone-360/Storage.cpp
