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
}

void Processor::initialise()
{
}

void Processor::start()
{
}

void Processor::stop() noexcept
{
}

void Processor::deinitialise() noexcept
{
}

void Processor::disconnect() noexcept
{
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
