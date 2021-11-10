//
//  emuone-cereon/Processor.cpp
//
//  cereon::Processor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction - from derived singleton classes only
Processor::Processor(const QString & name, Features features, InstructionSet * instructionSet,
                     core::ClockFrequency clockFrequency, util::ByteOrder byteOrder)
    :   core::Component(name),
        _features(features),
        _instructionSet(instructionSet),
        _clockFrequency(clockFrequency),
        _byteOrder(byteOrder)
{
    Q_ASSERT(_instructionSet != nullptr);
}

Processor::~Processor()
{
}

//////////
//  core::Component (state control) - all thread-safe
Processor::State Processor::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Processor::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Processor::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Processor::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Processor::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Processor::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Processor::disconnect() noexcept
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
//  core::Component (serialisation)
void Processor::serialiseConfiguration(QDomElement & configurationElement) const
{
}

void Processor::deserialiseConfiguration(QDomElement & configurationElement)
{
}

//  End of emuone-cereon/Processor.cpp
