//
//  emuone-360/Ibm360Storage.cpp
//
//  Ibm360Storage class implementation
//
//////////
#include "emuone-ibm360/API.hpp"

//////////
//  Construction/destruction
Ibm360Storage::Ibm360Storage(const QString & name, const MemorySize & size)
    :   Component(name),
        _size(size)
{
}

Ibm360Storage::~Ibm360Storage()
{
}

//////////
//  Component
ComponentType * Ibm360Storage::getType() const
{
    return Ibm360Storage::Type::getInstance();
}

ComponentEditor * Ibm360Storage::createEditor(QWidget * parent)
{
    return new Ibm360StorageEditor(this, parent);
}

QString Ibm360Storage::getShortStatus() const
{
    return _size.toDisplayString();
}

//////////
//  Component (state control) - all thread-safe
Component::State Ibm360Storage::getState() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Ibm360Storage::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Ibm360Storage::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Ibm360Storage::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Ibm360Storage::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Ibm360Storage::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }


    //  Done
    _state = State::Connected;
}

void Ibm360Storage::disconnect() noexcept
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
//  Operations
bool Ibm360Storage::isValidSize(const MemorySize & size)
{
    return (size.toBytes() % 2048) == 0 && size.toBytes() >= (4 * 1024) && size.toBytes() <= (16 * 1024 * 1024);
}

//////////
//  Ibm360Storage::Type
IMPLEMENT_SINGLETON(Ibm360Storage::Type)
Ibm360Storage::Type::Type() {}
Ibm360Storage::Type::~Type() {}

QString Ibm360Storage::Type::getMnemonic() const
{
    return "Ibm360Storage";
}

QString Ibm360Storage::Type::getDisplayName() const
{
    return "IBM/360 main storage";
}

ComponentCategory * Ibm360Storage::Type::getCategory() const
{
    return StandardComponentCategories::Memory::getInstance();
}

bool Ibm360Storage::Type::isCompatibleWith(Architecture * architecture) const
{
    return architecture == Ibm360Architecture::getInstance();
}

Ibm360Storage * Ibm360Storage::Type::createComponent()
{
    return new Ibm360Storage("Main storage", MemorySize(MemorySize::Unit::KB, 128));
}

//  End of emuone-360/Ibm360Storage.cpp
