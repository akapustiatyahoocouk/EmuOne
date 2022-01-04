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
Processor::Processor(const QString & name,
                     core::ClockFrequency clockFrequency, util::ByteOrder byteOrder,
                     uint8_t processorId, bool primary, uint64_t bootstrapIp)
    :   core::Component(name),
        _clockFrequency(clockFrequency),
        _byteOrder(byteOrder),
        _processorId(processorId),
        _primary(primary),
        _bootstrapIp(bootstrapIp)
{
}

Processor::~Processor()
{
}

void Processor::addCore(ProcessorCore * core)
{
    Q_ASSERT(core != nullptr);
    Q_ASSERT(core->processor() == this);

    if (_mapIdsToCores.contains(core->coreId()) && _mapIdsToCores[core->coreId()] != core)
    {   //  OOPS! Core ID conflict!
        throw core::VirtualApplianceException("Core ID conflict");
    }
    _mapIdsToCores.insert(core->coreId(), core);
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

    //  Exactly 1 primary processor must be defined
    QList<Processor*> processors = this->virtualAppliance()->findComponentsByRole<Processor>();
    int numPrimaryProcessors = 0;
    for (Processor * processor : processors)
    {
        if (processor->primary())
        {
            numPrimaryProcessors++;
        }
    }
    if (numPrimaryProcessors == 0)
    {
        throw core::VirtualApplianceException("No primary processor");
    }
    if (numPrimaryProcessors > 1)
    {
        throw core::VirtualApplianceException("Multiple primary processors");
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

    //  At most 1 "monitor" can be defined
    QList<Monitor*> monitors = this->virtualAppliance()->findComponentsByRole<Monitor>();
    if (monitors.size() > 1)
    {
        throw core::VirtualApplianceException("cereon::Processor found multiple Monitors");
    }

    //  Make the assigments
    _memoryBus = memoryBuses[0];
    _ioBus = ioBuses[0];
    _monitor = (monitors.size() == 0) ? nullptr : monitors[0];


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

    if (_mapIdsToCores.size() == 0)
    {   //  OOPS! Can't be!
        throw new core::VirtualApplianceException("Processor has no cores");
    }
    int numPrimaryCores = 0;
    for (ProcessorCore * core : _mapIdsToCores)
    {
        if (core->primary())
        {
            numPrimaryCores++;
        }
    }
    if (numPrimaryCores == 0)
    {
        throw new core::VirtualApplianceException("Processor has no primary core");
    }
    if (numPrimaryCores > 1)
    {
        throw new core::VirtualApplianceException("Processor has multiple primary cores");
    }

    _cores = new ProcessorCore*[static_cast<size_t>(_mapIdsToCores.size())];
    _numCores = 0;
    for (ProcessorCore * core : _mapIdsToCores)
    {
        _cores[_numCores++] = core;
    }

    for (int i = 0; i < _numCores; i++)
    {
        _cores[i]->initialise();    //  TODO what if it throws ?
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

    for (int i = 0; i < _numCores; i++)
    {
        _cores[i]->start();    //  TODO what if it throws ?
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

    for (int i = 0; i < _numCores; i++)
    {
        _cores[i]->stop();
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

    for (int i = 0; i < _numCores; i++)
    {
        _cores[i]->deinitialise();
    }

    delete [] _cores;
    _cores = 0;
    _numCores = 0;

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
    _monitor = nullptr;

    //  Done
    _state = State::Constructed;
}

//////////
//  core::Component (serialisation)
void Processor::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("ClockFrequency", util::toString(_clockFrequency));
    configurationElement.setAttribute("ByteOrder", util::toString(_byteOrder));
    configurationElement.setAttribute("ProcessorId", util::toString(_processorId));
    configurationElement.setAttribute("Primary", util::toString(_primary));
    configurationElement.setAttribute("BootstrapIp", util::toString(_bootstrapIp, "%016X"));
}

void Processor::deserialiseConfiguration(QDomElement & configurationElement)
{
    util::fromString(configurationElement.attribute("ClockFrequency"), _clockFrequency);
    util::fromString(configurationElement.attribute("ByteOrder"), _byteOrder);
    util::fromString(configurationElement.attribute("ProcessorId"), _processorId);
    util::fromString(configurationElement.attribute("Primary"), _primary);
    util::fromString(configurationElement.attribute("BootstrapIp"), "%X", _bootstrapIp);
}

//////////
//  core::IClockedComponentAspect
core::ClockFrequency Processor::clockFrequency() const
{
    return _clockFrequency;
}

void Processor::onClockTick()
{
    for (int i = 0; i < _numCores; i++)
    {
        _cores[i]->onClockTick();
    }
}

//////////
//  Operations
ProcessorCoreList Processor::cores() const
{
    return _mapIdsToCores.values();
}

//  End of emuone-cereon/Processor.cpp
