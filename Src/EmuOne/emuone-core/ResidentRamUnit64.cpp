//
//  emuone-core/ResidentRamUnit64.cpp
//
//  The core::ResidentRamUnit64 class implementation
//
//////////
#include "emuone-core/API.hpp"
#include "emuone-core/ResidentRamUnit64Editor.hpp"
using namespace core;

//////////
//  Construction/destruction
ResidentRamUnit64::ResidentRamUnit64(const QString & name, uint64_t startAddress, const MemorySize & size)
    :   MemoryUnit64(name, startAddress, size)
{
}

ResidentRamUnit64::~ResidentRamUnit64()
{
}

//////////
//  Component
ComponentEditor * ResidentRamUnit64::createEditor(QWidget * parent)
{
    ResidentRamUnit64Editor * editor = new ResidentRamUnit64Editor(this, parent);
    _editors.append(editor);
    return editor;
}

QString ResidentRamUnit64::shortStatus() const
{
    return this->size().toString() + " @ " + ("0000000000000000" + QString::number(startAddress(), 16)).right(16).toUpper();
}

ComponentUi * ResidentRamUnit64::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
Component::State ResidentRamUnit64::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void ResidentRamUnit64::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void ResidentRamUnit64::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    if ((size_t)size().toBytes() != size().toBytes())
    {   //  OOPS! Size overflow!
        throw VirtualApplianceException("Resident RAM unit too large");
    }

    try
    {
        size_t numBytes = (size_t)size().toBytes();
        _data = new uint8_t[numBytes];
        memset(_data, 0, numBytes);
    }
    catch (...)
    {   //  OOPS! Not enough host RAM! "_data" remains "nullptr".
        throw VirtualApplianceException("Resident RAM unit too large");
    }

    //  Done
    _state = State::Initialised;
}

void ResidentRamUnit64::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void ResidentRamUnit64::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void ResidentRamUnit64::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    delete [] _data;
    _data = nullptr;

    //  Done
    _state = State::Connected;
}

void ResidentRamUnit64::disconnect() noexcept
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
//  Operations (serialisation)

//  End of emuone-core/ResidentRamUnit64.cpp
