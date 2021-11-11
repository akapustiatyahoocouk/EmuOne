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
        registers(),
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

    //  Processor needs a) memory but and b) I/O bus
    QList<MemoryBus*> memoryBuses = this->virtualAppliance()->findComponentsByRole<MemoryBus>();
    if (memoryBuses.size() == 0)
    {
        throw core::VirtualApplianceException("cereon::Processor cannot find a MemoryBus");
    }
    if (memoryBuses.size() > 1)
    {
        throw core::VirtualApplianceException("cereon::Processor found multiple MemoryBuses");
    }

    QList<IoBus*> ioBuses = this->virtualAppliance()->findComponentsByRole<IoBus>();
    if (ioBuses.size() == 0)
    {
        throw core::VirtualApplianceException("cereon::Processor cannot find an IoBus");
    }
    if (ioBuses.size() > 1)
    {
        throw core::VirtualApplianceException("cereon::Processor found multiple IoBuses");
    }

    _memoryBus = memoryBuses[0];
    _ioBus = ioBuses[0];

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

    _memoryBus = nullptr;
    _ioBus = nullptr;

    //  Done
    _state = State::Constructed;
}

//////////
//  core::Component (serialisation)
void Processor::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void Processor::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//  End of emuone-cereon/Processor.cpp
