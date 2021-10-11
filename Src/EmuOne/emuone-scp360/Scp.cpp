//
//  emuone-scp360/Scp.cpp
//
//  The scp360::Scp class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;


//////////
//  Construction/destruction
Scp::Scp(const QString & name)
    :   ibm360::Monitor(name)
{
}

Scp::~Scp()
{
}

//////////
//  Component
Scp::Type * Scp::type() const
{
    return Scp::Type::getInstance();
}

ComponentEditor * Scp::createEditor(QWidget * parent)
{
    return nullptr;
}

QString Scp::shortStatus() const
{
    return "TODO";
}

core::ComponentUi * Scp::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Scp::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Scp::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Scp::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Scp::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Scp::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Scp::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }


    //  Done
    _state = State::Connected;
}

void Scp::disconnect() noexcept
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
void Scp::serialiseConfiguration(QDomElement & configurationElement) const
{
}

void Scp::deserialiseConfiguration(QDomElement & configurationElement)
{
}

//////////
//  scp360::Scp::Type
IMPLEMENT_SINGLETON(Scp::Type)
Scp::Type::Type() {}
Scp::Type::~Type() {}

QString Scp::Type::mnemonic() const
{
    return "scp360::Scp";
}

QString Scp::Type::displayName() const
{
    return "SCP/360";
}

core::ComponentCategory * Scp::Type::category() const
{
    return core::StandardComponentCategories::Software::getInstance();
}

bool Scp::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Scp * Scp::Type::createComponent()
{
    return new Scp("SCP/360");
}

//  End of emuone-scp360/Scp.cpp
