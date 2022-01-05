//
//  emuone-hadesos-cereon/Adaptor.cpp
//
//  The emuone::hadesos::Adaptor class implementation
//
//////////
#include "emuone-hadesos-cereon/API.hpp"
using namespace hadesos::cereon;

//////////
//  Construction/destruction
Adaptor::Adaptor(HadesOs * hadesOs)
    :   core::Adaptor(hadesOs->type()->displayName() + " adaptor", hadesOs)
{
}

Adaptor::~Adaptor()
{
}

//////////
//  core::Adaptor
core::AdaptorType * Adaptor::type() const
{
    return Adaptor::Type::instance();
}

core::AdaptorEditor * Adaptor::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString Adaptor::shortStatus() const
{
    return "The HADES OS / Cereon adaptor is operational";
}

core::AdaptorUi * Adaptor::createUi()
{
    return nullptr;
}

//////////
//  core::Adaptor (state control) - all thread-safe
Adaptor::State Adaptor::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Adaptor::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Adaptor::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Adaptor::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Adaptor::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Adaptor::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Adaptor::disconnect() noexcept
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
//  core::Adaptor (serialisation)
void Adaptor::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
    // TODO
}

void Adaptor::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
    // TODO
}

//  End of emuone-hadesos-cereon/Adaptor.cpp
