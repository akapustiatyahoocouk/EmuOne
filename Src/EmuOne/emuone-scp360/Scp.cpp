//
//  emuone-scp360/Scp.cpp
//
//  The scp360::Scp class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;


//////////
//  Construction/destruction
Scp::Scp(const QString & name)
    :   ibm360::Monitor(name)
{
}

Scp::~Scp()
{
}

//////////
//  Component
Scp::Type * Scp::type() const
{
    return Scp::Type::getInstance();
}

ComponentEditor * Scp::createEditor(QWidget * parent)
{
    return nullptr;
}

QString Scp::shortStatus() const
{
    return "TODO";
}

core::ComponentUi * Scp::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Scp::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Scp::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Discover devices
    QSet<core::Component*> adaptedComponents;
    for (core::Adaptor * adaptor : virtualAppliance()->adaptors())
    {
        adaptedComponents.insert(adaptor->adaptedComponent());
    }
    for (Device * device: virtualAppliance()->findComponents<Device>())
    {
        if (!adaptedComponents.contains(device))
        {
            _registerDevice(device);
        }
    }
    for (Device * device : virtualAppliance()->findAdaptors<Device>())
    {
        _registerDevice(device);
    }

    //  Done
    _state = State::Connected;
}

void Scp::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _createDeviceDrivers();

    //  Done
    _state = State::Initialised;
}

void Scp::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Start the worker thread
    _workerThread = new _WorkerThread(this);
    _workerThread->start();

    //  Done
    _state = State::Running;
}

void Scp::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Stop the worker thread
    _workerThread->requestStop();   //  this stops it eventually
    _workerThread = nullptr;

    //  Done
    _state = State::Initialised;
}

void Scp::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _destroyDeviceDrivers();

    //  Done
    _state = State::Connected;
}

void Scp::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _devices.clear();

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void Scp::serialiseConfiguration(QDomElement & configurationElement) const
{
}

void Scp::deserialiseConfiguration(QDomElement & configurationElement)
{
}

//////////
//  scp360::Scp::Type
IMPLEMENT_SINGLETON(Scp::Type)
Scp::Type::Type() {}
Scp::Type::~Type() {}

QString Scp::Type::mnemonic() const
{
    return "scp360::Scp";
}

QString Scp::Type::displayName() const
{
    return "SCP/360";
}

core::ComponentCategory * Scp::Type::category() const
{
    return core::StandardComponentCategories::Software::getInstance();
}

bool Scp::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Scp * Scp::Type::createComponent()
{
    return new Scp("SCP/360");
}

//////////
//  Implementation helpers
void Scp::_registerDevice(Device * device)
{
    if (_devices.contains(device->address()))
    {   //  OOPS!
        throw core::VirtualApplianceException("Device conflict at address " +
                                              ("000" + QString::number(device->address(), 16)).right(3).toUpper());
    }
    _devices.insert(device->address(), device);
}

void Scp::_createDeviceDrivers()
{
    for (Device * device : _devices)
    {
        DeviceDriver * deviceDriver = DeviceDriver::create(device);
        _deviceDrivers.insert(device->address(), deviceDriver);
    }
}

void Scp::_destroyDeviceDrivers()
{
    for (DeviceDriver * deviceDriver : _deviceDrivers)
    {
        delete deviceDriver;
    }
    _deviceDrivers.clear();
}

//////////
//  Scp::_WorkerThread
Scp::_WorkerThread::_WorkerThread(Scp *const scp)
    :   _scp(scp)
{
}

Scp::_WorkerThread::~_WorkerThread()
{
}

void Scp::_WorkerThread::run()
{
    //  Wait until VA is "running" - that means all Components and Adapters are Running
    while (_scp->virtualAppliance()->state() != core::VirtualAppliance::State::Running)
    {
        QThread::msleep(10);
    }
    //  Create the INIT process
    InitProcess * initProcess = new InitProcess(_scp, (uint16_t)0x0001);
    _scp->_processes.append(initProcess);
    initProcess->start();
    //  Go!
    while (!_stopRequested)
    {
    }
}

//  End of emuone-scp360/Scp.cpp
