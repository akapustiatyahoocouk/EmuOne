//
//  emuone-hadesos/HadesOs.cpp
//
//  The hadesos::HadesOs class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos;

//////////
//  Construction/destruction
HadesOs::HadesOs(const QString & name)
    :   core::Component(name)
{
}

HadesOs::~HadesOs()
{
}

//////////
//  Component
HadesOs::Type * HadesOs::type() const
{
    return HadesOs::Type::instance();
}

core::ComponentEditor * HadesOs::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString HadesOs::shortStatus() const
{
    return "HADES OS is operational";
}

core::ComponentUi * HadesOs::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
HadesOs::State HadesOs::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void HadesOs::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void HadesOs::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void HadesOs::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void HadesOs::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void HadesOs::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void HadesOs::disconnect() noexcept
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
void HadesOs::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void HadesOs::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//  End of emuone-hadesos/HadesOs.cpp
