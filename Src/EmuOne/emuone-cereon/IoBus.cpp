//
//  emuone-cereon/IoBus.cpp
//
//  The cereon::IoBus class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
IoBus::IoBus(const QString & name)
    :  core::Component(name)
{
}

IoBus::~IoBus()
{
}

//////////
//  core::Component
core::ComponentType * IoBus::type() const
{
    return Type::instance();
}

core::ComponentEditor * IoBus::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString IoBus::shortStatus() const
{
    return "";
}

core::ComponentUi * IoBus::createUi()
{
    return nullptr;
}

//////////
//  core::Component (state control) - all thread-safe
IoBus::State IoBus::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void IoBus::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Connect all I/O ports
    for (IIoBusClient * ioBusClient : this->virtualAppliance()->findComponentsByRole<IIoBusClient>())
    {
        for (IIoPort * ioPort : ioBusClient->ioPorts())
        {
            if (_ioPorts.contains(ioPort->address()))
            {   //  OOPS! Cleanup & abort
                _ioPorts.clear();
                _byteIoPorts.clear();
                _halfWordIoPorts.clear();
                _wordIoPorts.clear();
                _longWordIoPorts.clear();
                throw core::VirtualApplianceException("I/O bus conflict on port " + QString::number(static_cast<unsigned>(ioPort->address()), 16));
            }
            _ioPorts.insert(ioPort->address(), ioPort);
            if (IByteIoPort * byteIoPort = dynamic_cast<IByteIoPort*>(ioPort))
            {
                _byteIoPorts.insert(byteIoPort->address(), byteIoPort);
            }
            if (IHalfWordIoPort * halfWordIoPort = dynamic_cast<IHalfWordIoPort*>(ioPort))
            {
                _halfWordIoPorts.insert(halfWordIoPort->address(), halfWordIoPort);
            }
            if (IWordIoPort * wordIoPort = dynamic_cast<IWordIoPort*>(ioPort))
            {
                _wordIoPorts.insert(wordIoPort->address(), wordIoPort);
            }
            if (ILongWordIoPort * longWordIoPort = dynamic_cast<ILongWordIoPort*>(ioPort))
            {
                _longWordIoPorts.insert(longWordIoPort->address(), longWordIoPort);
            }
        }
    }

    //  Done
    _state = State::Connected;
}

void IoBus::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void IoBus::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void IoBus::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void IoBus::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void IoBus::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _ioPorts.clear();
    _byteIoPorts.clear();
    _halfWordIoPorts.clear();
    _wordIoPorts.clear();
    _longWordIoPorts.clear();

    //  Done
    _state = State::Constructed;
}

//////////
//  core::Component (serialisation)
void IoBus::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void IoBus::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//////////
//  IoBus::Type
IMPLEMENT_SINGLETON(IoBus::Type)
IoBus::Type::Type() {}
IoBus::Type::~Type() {}

QString IoBus::Type::mnemonic() const
{
    return "Cereon-IoBus";
}

QString IoBus::Type::displayName() const
{
    return "Cereon I/O Bus";
}

core::ComponentCategory * IoBus::Type::category() const
{
    return core::StandardComponentCategories::Motherboard::instance();
}

bool IoBus::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return dynamic_cast<const CereonArchitecture*>(architecture) != nullptr;
}


core::Component * IoBus::Type::createComponent()
{
    return new IoBus("I/O bus");
}

//  End of emuone-cereon/IoBus.cpp
