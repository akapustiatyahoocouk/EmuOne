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
    :   ibm360::Monitor(name),
        //  Subsystems
        _objectManager(this)
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

    //  Discover hardware devices
    QSet<core::Component*> adaptedComponents;
    for (core::Adaptor * adaptor : virtualAppliance()->adaptors())
    {
        adaptedComponents.insert(adaptor->adaptedComponent());
    }
    for (ibm360::Device * hardwareDevice : virtualAppliance()->findComponents<ibm360::Device>())
    {
        if (!adaptedComponents.contains(hardwareDevice))
        {
            _registerHardwareDevice(hardwareDevice);
        }
    }
    for (ibm360::Device * hardwareDevice : virtualAppliance()->findAdaptors<ibm360::Device>())
    {
        _registerHardwareDevice(hardwareDevice);
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

    try
    {
        _createDevicesAndDeviceDrivers();
    }
    catch (...)
    {   //  OOPS! Cleanup & re-throw
        _destroyDevicesAndDeviceDrivers();
        throw;
    }

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

    _destroyDevicesAndDeviceDrivers();

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

    _hardwareDevices.clear();

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
//  ibm360::Monitor - interrupt handling.
void Scp::onIoInterruption(uint16_t interruptionCode)
{
}

void Scp::onProgramInterruption(uint16_t interruptionCode)
{
}

void Scp::onSvcInterruption(uint16_t interruptionCode)
{   //  Must post a SystemCall to SCP's incoming events queue
    _events.enqueue(new _SystemCallEvent(nullptr));
}

void Scp::onExternalInterruption(uint16_t interruptionCode)
{
}

void Scp::onMachineCheckInterruption(uint16_t interruptionCode)
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
void Scp::_registerHardwareDevice(ibm360::Device * hardwareDevice)
{
    if (_hardwareDevices.contains(hardwareDevice->address()))
    {   //  OOPS!
        throw core::VirtualApplianceException("Device conflict at address " +
                                              ("000" + QString::number(hardwareDevice->address(), 16)).right(3).toUpper());
    }
    _hardwareDevices.insert(hardwareDevice->address(), hardwareDevice);
}

void Scp::_createDevicesAndDeviceDrivers()
{
    //  Create PhysicalDevices for all hardware devices, instantiating DeviceDrivers as necessary
    QMap<QString, DeviceDriver*> deviceDriverMap;
    for (ibm360::Device * hardwareDevice : _hardwareDevices)
    {
        QString hardwareDeviceTypeName = typeid(*hardwareDevice).name();
        DeviceDriver * deviceDriver;
        if (deviceDriverMap.contains(hardwareDeviceTypeName))
        {   //  Use existing driver
            deviceDriver = deviceDriverMap[hardwareDeviceTypeName];
        }
        else
        {   //  Need a new driver
            try
            {
                deviceDriver = DeviceDriver::createHardwareDeviceDriver(hardwareDevice);  //  throws VirtualApplianceException on error
            }
            catch (core::VirtualApplianceException & ex)
            {   //  OOPS! Cleanup...
                //  ...and re-throw
                throw;
            }
            deviceDriverMap.insert(hardwareDeviceTypeName, deviceDriver);
        }
        PhysicalDevice * physicalDevice;
        ErrorCode err = _objectManager.createPhysicalDevice(hardwareDevice, deviceDriver->deviceFlags(), deviceDriver, physicalDevice);
        if (err != ErrorCode::ERR_OK)
        {   //  OOPS! TODO proper error message
            throw core::VirtualApplianceException("Could not create physical device " + hardwareDevice->type()->displayName());
        }
        _deviceDrivers.insert(physicalDevice, deviceDriver);
    }

    //  Discover "operator's console" device
    Q_ASSERT(_operatorsConsole == nullptr);
    for (Device * device : _deviceDrivers.keys())
    {
        if (PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device))
        {
            ibm360::Device * hardwareDevice = physicalDevice->hardwareDevice();
            if (dynamic_cast<ibm360::Ibm2741*>(hardwareDevice))
            {   //  This hardware device CAN be used as an operator's console
                if (_operatorsConsole == nullptr ||
                    hardwareDevice->address() < _operatorsConsole->hardwareDevice()->address())
                {   //  ...and it's the new best
                    _operatorsConsole = physicalDevice;
                }
            }
        }
    }
    if (_operatorsConsole == nullptr)
    {
        throw core::VirtualApplianceException("Operator console device could not be found");
    }
}

void Scp::_destroyDevicesAndDeviceDrivers()
{
    _operatorsConsole = nullptr;
    for (DeviceDriver * deviceDriver : _deviceDrivers)
    {
        delete deviceDriver;
    }
    _deviceDrivers.clear();
}

//////////
//  Event handling
void Scp::_handleSystemCallEvent(_SystemCallEvent * event)
{
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
    EmulatedProcess * initProcess = nullptr;
    _scp->_objectManager.createEmulatedProcess(InitProcess::InitApplication::getInstance()->mnemonic(),
                                               Process::Flags::System,
                                               nullptr,
                                               InitProcess::InitApplication::getInstance(),
                                               initProcess);
    initProcess->start();
    //  Go!
    while (!_stopRequested)
    {   //  Wait for next control event...
        _Event * event;
        if (!_scp->_events.tryDequeue(event, 50))
        {   //  Nothing to do
            continue;
        }
        if (_SystemCallEvent * systemCallEvent = dynamic_cast<_SystemCallEvent*>(event))
        {
            _scp->_handleSystemCallEvent(systemCallEvent);
        }
        delete event;
    }
}

//  End of emuone-scp360/Scp.cpp
