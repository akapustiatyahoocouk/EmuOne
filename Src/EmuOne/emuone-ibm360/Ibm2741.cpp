//
//  emuone-360/Ibm2741.cpp
//
//  ibm360::Ibm2741 class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm2741::Ibm2741(const QString & name, uint16_t address)
    :   Device(name, address)
{
}

Ibm2741::~Ibm2741()
{
}

//////////
//  Component
core::ComponentType * Ibm2741::getType() const
{
    return Ibm2741::Type::getInstance();
}

ComponentEditor * Ibm2741::createEditor(QWidget * parent)
{
    return new Ibm2741Editor(this, parent);;
}

QString Ibm2741::getShortStatus() const
{
    return "@" + ("000" + QString::number(getAddress(), 16)).right(3).toUpper();
}

core::ComponentUi * Ibm2741::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Ibm2741::getState() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Ibm2741::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Ibm2741::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Ibm2741::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Ibm2741::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Ibm2741::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }


    //  Done
    _state = State::Connected;
}

void Ibm2741::disconnect() noexcept
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
void Ibm2741::serialiseConfiguration(QDomElement & configurationElement) const
{
    Device::serialiseConfiguration(configurationElement);
}

void Ibm2741::deserialiseConfiguration(QDomElement & configurationElement)
{
    Device::deserialiseConfiguration(configurationElement);
}

//////////
//  Ibm2741::Type
IMPLEMENT_SINGLETON(Ibm2741::Type)
Ibm2741::Type::Type() {}
Ibm2741::Type::~Type() {}

QString Ibm2741::Type::getMnemonic() const
{
    return "ibm360::Ibm2741";
}

QString Ibm2741::Type::getDisplayName() const
{
    return "IBM 2741 terminal";
}

core::ComponentCategory * Ibm2741::Type::getCategory() const
{
    return core::StandardComponentCategories::Terminals::getInstance();
}

bool Ibm2741::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Ibm2741 * Ibm2741::Type::createComponent()
{
    return new Ibm2741("IBM 2741", 0x000);
}

//  End of emuone-360/Ibm2741.cpp
