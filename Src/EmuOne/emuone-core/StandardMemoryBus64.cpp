//
//  emuone-core/StandardMemoryBus64.cpp
//
//  The core::StandardMemoryBus64 class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
StandardMemoryBus64::StandardMemoryBus64(const QString & name)
    :   Component(name)
{
}

StandardMemoryBus64::~StandardMemoryBus64()
{
}

//////////
//  Component
ComponentEditor * StandardMemoryBus64::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString StandardMemoryBus64::shortStatus() const
{
    return this->type()->displayName();
}

ComponentUi * StandardMemoryBus64::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
Component::State StandardMemoryBus64::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void StandardMemoryBus64::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _numMemoryUnits = 0;
    _memoryUnits = new StandardMemoryUnit64*[1];

    //  Discover compatible memory units and attach them to this memory bus
    StandardMemoryUnit64List memoryUnits = findMemoryUnits();
    for (int i = 0; i < memoryUnits.size(); i++)
    {
        try
        {
            attachMemoryUnit(memoryUnits[i]);
        }
        catch (VirtualApplianceException &)
        {   //  OOPS! Cleanup & re-throw
            _numMemoryUnits = 0;
            delete [] _memoryUnits;
            _memoryUnits = nullptr;
            throw;
        }
    }

    //  Done
    _state = State::Connected;
}

void StandardMemoryBus64::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void StandardMemoryBus64::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void StandardMemoryBus64::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void StandardMemoryBus64::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void StandardMemoryBus64::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _numMemoryUnits = 0;
    delete [] _memoryUnits;
    _memoryUnits = nullptr;

    //  Done
    _state = State::Constructed;
}

//////////
//  Operations (serialisation)
void StandardMemoryBus64::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void StandardMemoryBus64::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//////////
//  Operations
void StandardMemoryBus64::attachMemoryUnit(MemoryUnit64 * memoryUnit)
{
    Q_ASSERT(memoryUnit != nullptr);

    //  Already attached ?
    for (size_t i = 0; i < _numMemoryUnits; i++)
    {
        if (_memoryUnits[i] == memoryUnit)
        {   //  Yes
            return;
        }
    }

    //  Make sure there are no address range conflicts
    if (memoryUnit->size().toBytes() == 0)
    {
        throw VirtualApplianceException("Memory unit is empty");
    }
    if (memoryUnit->startAddress() + (memoryUnit->size().toBytes() - 1) < memoryUnit->startAddress())
    {
        throw VirtualApplianceException("Memory unit overflows the address range");
    }
    for (size_t i = 0; i < _numMemoryUnits; i++)
    {
        uint64_t intersectionStart = qMax(_memoryUnits[i]->startAddress(), memoryUnit->startAddress());
        uint64_t intersectionEnd = qMin(_memoryUnits[i]->endAddress(), memoryUnit->endAddress());
        if (intersectionStart <= intersectionEnd)
        {
            throw VirtualApplianceException("Memory unit conflict in address range " +
                                            ("0000000000000000" + QString::number(intersectionStart, 16)).right(16).toUpper() +
                                            ".." +
                                            ("0000000000000000" + QString::number(intersectionEnd, 16)).right(16).toUpper());
        }
    }

    //  Re-allocate...
    MemoryUnit64 ** newMemoryUnits = new MemoryUnit64*[_numMemoryUnits + 1];
    for (size_t i = 0; i < _numMemoryUnits; i++)
    {
        newMemoryUnits[i] = _memoryUnits[i];
    }
    newMemoryUnits[_numMemoryUnits] = memoryUnit;
    delete [] _memoryUnits;
    _memoryUnits = newMemoryUnits;
    _numMemoryUnits++;
    //  ...and keep sorted by start address
}

void StandardMemoryBus64::detachMemoryUnit(MemoryUnit64 * memoryUnit)
{
    Q_ASSERT(memoryUnit != nullptr);

    //  Attached ?
    size_t memoryUnitIndex = SIZE_MAX;
    for (size_t i = 0; i < _numMemoryUnits; i++)
    {
        if (_memoryUnits[i] == memoryUnit)
        {   //  Yes
            memoryUnitIndex = i;
            break;
        }
    }
    if (memoryUnitIndex == SIZE_MAX)
    {   //  Nothing to do
        return;
    }

    //  Re-allocate...
    MemoryUnit64 ** newMemoryUnits = new MemoryUnit64*[_numMemoryUnits - 1];
    size_t j = 0;
    for (size_t i = 0; i < _numMemoryUnits; i++)
    {
        if (i != memoryUnitIndex)
        {
            newMemoryUnits[j++] = _memoryUnits[i];
        }
    }
    _memoryUnits = newMemoryUnits;
    _numMemoryUnits--;
}

//  End of emuone-core/StandardMemoryBus64.cpp
