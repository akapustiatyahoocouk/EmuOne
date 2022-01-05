//
//  emuone-cereon/RomUnit.cpp
//
//  cereon::RomUnit class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
RomUnit::RomUnit(const QString & name, const core::MemorySize & size, uint64_t startAddress, const QString & contentFileName)
    :   core::Component(name),
        _size(size),
        _startAddress(startAddress),
        _contentFileName(contentFileName)
{
}

RomUnit::~RomUnit()
{
    delete [] _data;    //  ...just in case
}

//////////
//  core::Component
core::ComponentType * RomUnit::type() const
{
    return Type::instance();
}

core::ComponentEditor * RomUnit::createEditor(QWidget * parent)
{
    RomUnitEditor * editor = new RomUnitEditor(this, parent);
    _editors.append(editor);
    return editor;
}

QString RomUnit::shortStatus() const
{
    return util::toString(_size) + " ROM @ " + util::toString(_startAddress, "%016X");
}

core::ComponentUi * RomUnit::createUi()
{
    return nullptr;
}

//////////
//  core::Component (state control) - all thread-safe
RomUnit::State RomUnit::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void RomUnit::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void RomUnit::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    size_t dataSize = static_cast<size_t>(_size.toBytes());
    if (dataSize == 0 || dataSize != _size.toBytes())
    {   //  OOPS! Empty OR overflow
        throw core::VirtualApplianceException("Invalid or unsupported ROM unit size");
    }
    try
    {
        _data = new uint8_t[dataSize];
    }
    catch (...)
    {   //  OOPS! Out of RAM!
        throw core::VirtualApplianceException("Insufficient memory");
    }
    memset(_data, 0, dataSize);
    _dataSize = dataSize;

    //  Done
    _state = State::Initialised;
}

void RomUnit::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void RomUnit::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void RomUnit::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    delete [] _data;
    _data = nullptr;
    _dataSize = 0;

    //  Done
    _state = State::Connected;
}

void RomUnit::disconnect() noexcept
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
void RomUnit::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("StartAddress", util::toString(_startAddress, "%016X"));
    configurationElement.setAttribute("Size", util::toString(_size));
    configurationElement.setAttribute("ContentFileName", util::toString(_contentFileName));
}

void RomUnit::deserialiseConfiguration(QDomElement & configurationElement)
{
    util::fromString(configurationElement.attribute("StartAddress"), "%X", _startAddress);
    util::fromString(configurationElement.attribute("Size"), _size);
    util::fromString(configurationElement.attribute("ContentFileName"), _contentFileName);
}

//////////
//  MemoryUnit
RomUnit::AccessOutcome RomUnit::loadByte(uint64_t offset, uint8_t & value)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else
    {   //  Go!
        value = _data[static_cast<size_t>(offset)];
        return AccessOutcome::Success;
    }
}

RomUnit::AccessOutcome RomUnit::loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 1 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x01) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  Go!
        value = util::Memory::loadUInt16(_data + static_cast<size_t>(offset), byteOrder);
        return AccessOutcome::Success;
    }
}

RomUnit::AccessOutcome RomUnit::loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 3 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x03) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  Go!
        value = util::Memory::loadUInt32(_data + static_cast<size_t>(offset), byteOrder);
        return AccessOutcome::Success;
    }
}

RomUnit::AccessOutcome RomUnit::loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 7 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x07) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  Go!
        value = util::Memory::loadUInt64(_data + static_cast<size_t>(offset), byteOrder);
        return AccessOutcome::Success;
    }
}

RomUnit::AccessOutcome RomUnit::storeByte(uint64_t offset, uint8_t /*value*/)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else
    {   //  OOPS! Can't write to ROM!
        return AccessOutcome::AccessDenied;
    }
}

RomUnit::AccessOutcome RomUnit::storeHalfWord(uint64_t offset, util::ByteOrder /*byteOrder*/, uint16_t /*value*/)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 1 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x01) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  OOPS! Can't write to ROM!
        return AccessOutcome::AccessDenied;
    }
}

RomUnit::AccessOutcome RomUnit::storeWord(uint64_t offset, util::ByteOrder /*byteOrder*/, uint32_t /*value*/)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 3 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x03) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  OOPS! Can't write to ROM!
        return AccessOutcome::AccessDenied;
    }
}

RomUnit::AccessOutcome RomUnit::storeLongWord(uint64_t offset, util::ByteOrder /*byteOrder*/, uint64_t /*value*/)
{
    if (_data == nullptr)
    {   //  OOPS! No runtime state!
        return AccessOutcome::UnknownError;
    }
    else if (offset + 7 >= _dataSize)
    {   //  OOPS! Invalid address!
        return AccessOutcome::InvalidAddress;
    }
    else if ((offset & 0x07) != 0)
    {   //  OOPS! Not naturally aligned!
        return AccessOutcome::InvalidAlignment;
    }
    else
    {   //  OOPS! Can't write to ROM!
        return AccessOutcome::AccessDenied;
    }
}

//////////
//  BoundMemoryUnit

//////////
//  IMemoryBusClient
BoundMemoryUnitList RomUnit::memoryUnits()
{
    BoundMemoryUnitList result;
    result.append(this);
    return result;
}

//////////
//  Operations

//////////
//  RomUnit::Type
IMPLEMENT_SINGLETON(RomUnit::Type)
RomUnit::Type::Type() {}
RomUnit::Type::~Type() {}

QString RomUnit::Type::mnemonic() const
{
    return "Cereon-RomUnit";
}

QString RomUnit::Type::displayName() const
{
    return "Cereon ROM unit";
}

core::ComponentCategory * RomUnit::Type::category() const
{
    return core::StandardComponentCategories::Memory::instance();
}

bool RomUnit::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return dynamic_cast<const CereonArchitecture*>(architecture) != nullptr;
}

core::Component * RomUnit::Type::createComponent()
{
    return new RomUnit("ROM", core::MemorySize(core::MemorySize::Unit::MB, 1), UINT64_C(0xFFFFFFFFFFF00000), "./rom.bin");
}

//  End of emuone-cereon/RomUnit.cpp
