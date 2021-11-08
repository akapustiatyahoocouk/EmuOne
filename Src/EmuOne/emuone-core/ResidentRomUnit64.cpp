//
//  emuone-core/ResidentRomUnit64.cpp
//
//  The core::ResidentRomUnit64 class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
ResidentRomUnit64::ResidentRomUnit64(const QString & name, uint64_t startAddress, const MemorySize & size, const QString & contentFileName)
    :   MemoryUnit64(name, startAddress, size),
        _contentFileName(contentFileName)
{
}

ResidentRomUnit64::~ResidentRomUnit64()
{
}

//////////
//  Component
ComponentEditor * ResidentRomUnit64::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString ResidentRomUnit64::shortStatus() const
{
    return this->size().toString() + " @ " + ("0000000000000000" + QString::number(startAddress(), 16)).right(16).toUpper();
}

ComponentUi * ResidentRomUnit64::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
Component::State ResidentRomUnit64::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void ResidentRomUnit64::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void ResidentRomUnit64::initialise()
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

    size_t numBytes = (size_t)size().toBytes();
    try
    {
        _data = new uint8_t[numBytes];
        memset(_data, 0, numBytes);
    }
    catch (...)
    {   //  OOPS! Not enough host RAM! "_data" remains "nullptr".
        throw VirtualApplianceException("Resident RAM unit too large");
    }

    //  Load content
    QString contentFileName = this->virtualAppliance()->toAbsolutePath(_contentFileName);
    QFile file(contentFileName);
    if (!file.open(QFile::ReadOnly | QFile::ExistingOnly))
    {   //  OOPS!
        delete [] _data;
        _data = nullptr;
        throw VirtualApplianceException("Error reading " + contentFileName);
    }
    file.read((char*)_data, numBytes);
    file.close();

    //  Done
    _state = State::Initialised;
}

void ResidentRomUnit64::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void ResidentRomUnit64::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void ResidentRomUnit64::deinitialise() noexcept
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

void ResidentRomUnit64::disconnect() noexcept
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
void ResidentRomUnit64::serialiseConfiguration(QDomElement & configurationElement) const
{
    MemoryUnit64::serialiseConfiguration(configurationElement);
    configurationElement.setAttribute("ContentFileName", _contentFileName);
}

void ResidentRomUnit64::deserialiseConfiguration(QDomElement & configurationElement)
{
    MemoryUnit64::deserialiseConfiguration(configurationElement);
    _contentFileName = configurationElement.attribute("ContentFileName", _contentFileName);
}

//  End of emuone-core/ResidentRomUnit64.cpp
