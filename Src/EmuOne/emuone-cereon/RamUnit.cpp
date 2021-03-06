//
//  emuone-cereon/RamUnit.cpp
//
//  cereon::RamUnit class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
RamUnit::RamUnit(const QString & name, const core::MemorySize & size, uint64_t startAddress)
    :   core::Component(name),
        _size(size),
        _startAddress(startAddress)
{
}

RamUnit::~RamUnit()
{
    delete [] _data;    //  ...just in case
}

//////////
//  core::Component
core::ComponentType * RamUnit::type() const
{
    return Type::instance();
}

core::ComponentEditor * RamUnit::createEditor(QWidget * parent)
{
    RamUnitEditor * editor = new RamUnitEditor(this, parent);
    _editors.append(editor);
    return editor;
}

QString RamUnit::shortStatus() const
{
    return util::toString(_size) + " RAM @ " + util::toString(_startAddress, "%016X");
}

core::ComponentUi * RamUnit::createUi()
{
    return nullptr;
}

//////////
//  core::Component (state control) - all thread-safe
RamUnit::State RamUnit::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void RamUnit::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void RamUnit::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    size_t dataSize = static_cast<size_t>(_size.toBytes());
    if (dataSize == 0 || dataSize != _size.toBytes())
    {   //  OOPS! Empty OR overflow
        throw core::VirtualApplianceException("Invalid or unsupported RAM unit size");
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

void RamUnit::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void RamUnit::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void RamUnit::deinitialise() noexcept
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

void RamUnit::disconnect() noexcept
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
void RamUnit::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("StartAddress", util::toString(_startAddress, "%016X"));
    configurationElement.setAttribute("Size", util::toString(_size));
}

void RamUnit::deserialiseConfiguration(QDomElement & configurationElement)
{
    util::fromString(configurationElement.attribute("StartAddress"), "%X", _startAddress);
    util::fromString(configurationElement.attribute("Size"), _size);
}

//////////
//  MemoryUnit
RamUnit::AccessOutcome RamUnit::loadByte(uint64_t offset, uint8_t & value)
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

RamUnit::AccessOutcome RamUnit::loadHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t & value)
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

RamUnit::AccessOutcome RamUnit::loadWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t & value)
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

RamUnit::AccessOutcome RamUnit::loadLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t & value)
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

RamUnit::AccessOutcome RamUnit::storeByte(uint64_t offset, uint8_t value)
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
        _data[static_cast<size_t>(offset)] = value;
        return AccessOutcome::Success;
    }
}

RamUnit::AccessOutcome RamUnit::storeHalfWord(uint64_t offset, util::ByteOrder byteOrder, uint16_t value)
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
        util::Memory::storeUInt16(_data + static_cast<size_t>(offset), value, byteOrder);
        return AccessOutcome::Success;
    }
}

RamUnit::AccessOutcome RamUnit::storeWord(uint64_t offset, util::ByteOrder byteOrder, uint32_t value)
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
        util::Memory::storeUInt32(_data + static_cast<size_t>(offset), value, byteOrder);
        return AccessOutcome::Success;
    }
}

RamUnit::AccessOutcome RamUnit::storeLongWord(uint64_t offset, util::ByteOrder byteOrder, uint64_t value)
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
        util::Memory::storeUInt64(_data + static_cast<size_t>(offset), value, byteOrder);
        return AccessOutcome::Success;
    }
}

//////////
//  BoundMemoryUnit

//////////
//  IMemoryBusClient
BoundMemoryUnitList RamUnit::memoryUnits()
{
    BoundMemoryUnitList result;
    result.append(this);
    return result;
}

//////////
//  Operations

//////////
//  RamUnit::Type
IMPLEMENT_SINGLETON(RamUnit::Type)
RamUnit::Type::Type() {}
RamUnit::Type::~Type() {}

QString RamUnit::Type::mnemonic() const
{
    return "Cereon-RamUnit";
}

QString RamUnit::Type::displayName() const
{
    return "Cereon RAM unit";
}

core::ComponentCategory * RamUnit::Type::category() const
{
    return core::StandardComponentCategories::Memory::instance();
}

bool RamUnit::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return dynamic_cast<const CereonArchitecture*>(architecture) != nullptr;
}

core::Component * RamUnit::Type::createComponent()
{
    return new RamUnit("RAM", core::MemorySize(core::MemorySize::Unit::MB, 64), 0);
}

//  End of emuone-cereon/RamUnit.cpp
