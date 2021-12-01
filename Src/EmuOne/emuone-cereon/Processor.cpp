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
                     core::ClockFrequency clockFrequency, util::ByteOrder byteOrder,
                     uint8_t processorId, bool primary, uint64_t bootstrapIp)
    :   core::Component(name),
        _features(features),
        _instructionSet(instructionSet),
        _clockFrequency(clockFrequency),
        _byteOrder(byteOrder),
        _processorId(processorId),
        _primary(primary),
        _bootstrapIp(bootstrapIp)
{
    Q_ASSERT(_instructionSet != nullptr);
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

    //  Done
    _state = State::Constructed;
}

//////////
//  core::Component (serialisation)
void Processor::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("ClockFrequency", _clockFrequency.toString());
    configurationElement.setAttribute("ByteOrder", (_byteOrder == util::ByteOrder::BigEndian) ? "BigEndian" : "LittleEndian");
    configurationElement.setAttribute("ProcessorId", QString::number(static_cast<unsigned>(_processorId)));
    configurationElement.setAttribute("Primary", _primary ? "Yes" : "No");
    configurationElement.setAttribute("BootstrapIp", ("0000000000000000" + QString::number(_bootstrapIp, 16)).right(16).toUpper());
}

void Processor::deserialiseConfiguration(QDomElement & configurationElement)
{
    QString clockFrequencyString = configurationElement.attribute("ClockFrequency");
    _clockFrequency = core::ClockFrequency::fromString(clockFrequencyString, _clockFrequency);

    QString byteOrderString = configurationElement.attribute("ByteOrder");
    if (byteOrderString == "BigEndian")
    {
        _byteOrder = util::ByteOrder::BigEndian;
    }
    else if (byteOrderString == "LittleEndian")
    {
        _byteOrder = util::ByteOrder::LittleEndian;
    }

    QString processorIdString = configurationElement.attribute("ProcessorId");
    bool ok = false;
    unsigned processorId = processorIdString.toUInt(&ok);
    if (ok && processorId <= 255)
    {
        _processorId = static_cast<uint8_t>(processorId);
    }

    QString primaryString = configurationElement.attribute("Primary");
    if (primaryString == "Yes")
    {
        _primary = true;
    }
    else if (primaryString == "No")
    {
        _primary = false;
    }

    QString bootstrapIpString = configurationElement.attribute("BootstrapIp");
    uint64_t bootstrapIp = bootstrapIpString.toULongLong(&ok, 16);
    if (ok)
    {
        _bootstrapIp = bootstrapIp;
    }
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

//  End of emuone-cereon/Processor.cpp
