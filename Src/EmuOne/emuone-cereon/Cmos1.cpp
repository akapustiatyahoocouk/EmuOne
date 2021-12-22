//
//  emuone-cereon/Cmos1.cpp
//
//  The cereon::Cmos1 class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Constants
const core::Duration Cmos1::DefaultReadDelay = core::Duration(core::Duration::Unit::Microseconds, 1);
const core::Duration Cmos1::DefaultWriteDelay = core::Duration(core::Duration::Unit::Microseconds, 10);
const core::ClockFrequency Cmos1::DefaultClockFrequency(core::ClockFrequency::Unit::MHZ, 1);
const QString Cmos1::DefaultContentFilePath = "./cmos.bin";

//////////
//  Construction/destruction
Cmos1::Cmos1(const QString & name)
    :   core::Component(name),
        _statePort(this),
        _addressPort(this),
        _dataPort(this),
        _interruptMaskPort(this)
{
}

Cmos1::~Cmos1()
{
}

//////////
//  core::Component
core::ComponentType * Cmos1::type() const
{
    return Type::instance();
}

core::ComponentEditor * Cmos1::createEditor(QWidget * parent)
{
    return new Cmos1Editor(this, parent);
}

QString Cmos1::shortStatus() const
{
    return "";
}

core::ComponentUi * Cmos1::createUi()
{
    return nullptr;
}

//////////
//  core::Component (state control) - all thread-safe
Cmos1::State Cmos1::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Cmos1::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Cmos1::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    memset(_data, 0, 256);
    _currentAddress = 0;
    _interruptMask = 0;

    //  Done
    _state = State::Initialised;
}

void Cmos1::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Cmos1::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Cmos1::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    memset(_data, 0, 256);
    _currentAddress = 0;
    _interruptMask = 0;

    //  Done
    _state = State::Connected;
}

void Cmos1::disconnect() noexcept
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
void Cmos1::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("StatePortAddress", util::toString(_statePortAddress, "%04X"));
    configurationElement.setAttribute("AddressPortAddress", util::toString(_addressPortAddress, "%04X"));
    configurationElement.setAttribute("DataPortAddress", util::toString(_dataPortAddress, "%04X"));
    configurationElement.setAttribute("InterruptMaskPortAddress", util::toString(_interruptMaskPortAddress, "%04X"));
}

void Cmos1::deserialiseConfiguration(QDomElement & configurationElement)
{
    util::fromString(configurationElement.attribute("StatePortAddress"), "%X", _statePortAddress);

    bool addressOk = false;

    uint16_t statePortAddress = static_cast<uint16_t>(configurationElement.attribute("StatePortAddress").toUInt(&addressOk, 16));
    if (addressOk)
    {
        _statePortAddress = statePortAddress;
    }

    uint16_t addressPortAddress = static_cast<uint16_t>(configurationElement.attribute("AddressPortAddress").toUInt(&addressOk, 16));
    if (addressOk)
    {
        _addressPortAddress = addressPortAddress;
    }

    uint16_t dataPortAddress = static_cast<uint16_t>(configurationElement.attribute("DataPortAddress").toUInt(&addressOk, 16));
    if (addressOk)
    {
        _dataPortAddress = dataPortAddress;
    }

    uint16_t interruptMaskPortAddress = static_cast<uint16_t>(configurationElement.attribute("InterruptMaskPortAddress").toUInt(&addressOk, 16));
    if (addressOk)
    {
        _interruptMaskPortAddress = interruptMaskPortAddress;
    }
}

//////////
//  IIoBusClient
IoPortList Cmos1::ioPorts()
{
    IoPortList result = { &_statePort, &_addressPort, &_dataPort };
    return result;
}

//////////
//  Cmos1::Type
IMPLEMENT_SINGLETON(Cmos1::Type)
Cmos1::Type::Type() {}
Cmos1::Type::~Type() {}

QString Cmos1::Type::mnemonic() const
{
    return "Cereon-CMOS1";
}

QString Cmos1::Type::displayName() const
{
    return "Cereon CMOS1";
}

core::ComponentCategory * Cmos1::Type::category() const
{
    return core::StandardComponentCategories::Motherboard::instance();
}

bool Cmos1::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return dynamic_cast<CereonArchitecture*>(architecture) != nullptr;
}


core::Component * Cmos1::Type::createComponent()
{
    return new Cmos1("CMOS");
}

//////////
//  Cmos1::_StatePort
uint8_t Cmos1::_StatePort::readByte()
{
    return 0x07;    //  ON + ADDRESS_READY + DATA_READY
}

//////////
//  Cmos1::_AddressPort
uint8_t Cmos1::_AddressPort::readByte()
{
    return _cmos1->_currentAddress;
    //  TODO simulate write delay & "port ready" interrupts
}

void Cmos1::_AddressPort::writeByte(uint8_t value)
{
    _cmos1->_currentAddress = value;
    //  TODO simulate write delay & "port ready" interrupts
}

//////////
//  Cmos1::_DataPort
uint8_t Cmos1::_DataPort::readByte()
{
    return _cmos1->_data[_cmos1->_currentAddress];
    //  TODO simulate write delay & "port ready" interrupts
}

void Cmos1::_DataPort::writeByte(uint8_t value)
{
    _cmos1->_data[_cmos1->_currentAddress] = value;
    //  TODO simulate write delay & "port ready" interrupts
}

//////////
//  Cmos1::_InterruptMaskPort
uint8_t Cmos1::_InterruptMaskPort::readByte()
{
    return _cmos1->_interruptMask;
    //  TODO simulate write delay & "port ready" interrupts
}

void Cmos1::_InterruptMaskPort::writeByte(uint8_t value)
{
    _cmos1->_interruptMask = value;
    //  TODO simulate write delay & "port ready" interrupts
}

//  End of emuone-cereon/Cmos1.cpp
