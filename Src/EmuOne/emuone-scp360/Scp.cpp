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
        _objectManager(this),
        //  Interrupt handler
        _transferCompletionListener(this)
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

ComponentEditor * Scp::createEditor(QWidget * /*parent*/)
{
    return nullptr;
}

QString Scp::shortStatus() const
{
    return "V1.0";
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
    _workerThread->wait();
    _workerThread = nullptr;
    _objectManager.destroyAllObjects();


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
void Scp::serialiseConfiguration(QDomElement & /*configurationElement*/) const
{
}

void Scp::deserialiseConfiguration(QDomElement & /*configurationElement*/)
{
}

//////////
//  ibm360::Monitor - interrupt handling.
void Scp::onIoInterruption(uint16_t /*interruptionCode*/)
{
}

void Scp::onProgramInterruption(uint16_t /*interruptionCode*/)
{
}

void Scp::onSvcInterruption(uint16_t /*interruptionCode*/)
{   //  Must post a SystemCall to SCP's incoming events queue
    _events.enqueue(new _SystemCallEvent(nullptr));
}

void Scp::onExternalInterruption(uint16_t /*interruptionCode*/)
{
}

void Scp::onMachineCheckInterruption(uint16_t /*interruptionCode*/)
{
}

//////////
//  Operations
bool Scp::onKernelThread() const
{
    return QThread::currentThread() == _workerThread;
}

ErrorCode Scp::makeSystemCall(SystemCall * systemCall)
{
    Q_ASSERT(systemCall != nullptr);
    Q_ASSERT(systemCall->process()->scp() == this);
    Q_ASSERT(systemCall->process()->state() == Process::State::Active);
    Q_ASSERT(QThread::currentThread() != _workerThread);

    //  Send systemCall to worker thread for processing there
    _events.enqueue(new _SystemCallEvent(systemCall));
    return ErrorCode::ERR_OK;
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
            if (_isTerminal(hardwareDevice))
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
    for (PhysicalDevice * physicalDevice : _objectManager.physicalDevices())
    {
        _objectManager.destroyPhysicalDevice(physicalDevice);
    }
    _operatorsConsole = nullptr;

    QSet<DeviceDriver*> deviceDrivers;
    for (DeviceDriver * deviceDriver : _deviceDrivers)
    {
        deviceDrivers.insert(deviceDriver);
    }
    for (DeviceDriver * deviceDriver : deviceDrivers)
    {
        delete deviceDriver;
    }
    _deviceDrivers.clear();
}

bool Scp::_isTerminal(ibm360::Device * hardwareDevice)
{
    return (dynamic_cast<ibm360::Ibm2741*>(hardwareDevice) != nullptr);
}

//////////
//  Interrupt handling
void Scp::_TransferCompletionListener::transferCompleted(Device * device, uint32_t bytesTransferred, ErrorCode errorCode)
{
    _scp->_events.enqueue(new _TransferCompleteEvent(device, bytesTransferred, errorCode));
}

//////////
//  Event handling
void Scp::_handleSystemCallEvent(_SystemCallEvent * event)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(event != nullptr);

    Process * process = event->_systemCall->process();
    //  The process that made the system call is now "waiting"
    process->setState(Process::State::Waiting);
    //  Dispatch system call to an appropriate handler
    if (auto writeToOperatorSystemCall = dynamic_cast<WriteToOperatorSystemCall*>(event->_systemCall))
    {
        _handleWriteToOperatorSystemCall(writeToOperatorSystemCall);
    }
    else if (auto readFromOperatorSystemCall = dynamic_cast<ReadFromOperatorSystemCall*>(event->_systemCall))
    {
        _handleReadFromOperatorSystemCall(readFromOperatorSystemCall);
    }
    else if (auto openFileSystemCall = dynamic_cast<OpenFileSystemCall*>(event->_systemCall))
    {
        _handleOpenFileSystemCall(openFileSystemCall);
    }
    else if (auto writeToFileSystemCall = dynamic_cast<WriteToFileSystemCall*>(event->_systemCall))
    {
        _handleWriteToFileSystemCall(writeToFileSystemCall);
    }
    else if (auto readFromFileSystemCall = dynamic_cast<ReadFromFileSystemCall*>(event->_systemCall))
    {
        _handleReadFromFileSystemCall(readFromFileSystemCall);
    }
    else if (auto setEnvironmentVariableValueSystemCall = dynamic_cast<SetEnvironmentVariableValueSystemCall*>(event->_systemCall))
    {
        _handleSetEnvironmentVariableValueSystemCall(setEnvironmentVariableValueSystemCall);
    }
    else
    {   //  OOPS! Unknown system call
        _handleUnknownSystemCall(event->_systemCall);
    }
    //  If the system call hasn't been handled immediately, put it to
    //  the list of "system calls in progress"
    if (!event->_systemCall->outcomeKnown())
    {
        _systemCallsInProgress.insert(process, event->_systemCall);
        return;
    }
    //  The process that made the system call is now "ready" again
    //  or "suspended" if another process suspended the one that made
    //  the system call
    if (EmulatedProcess * emulatedProcess = dynamic_cast<EmulatedProcess*>(process))
    {   //  Must allow EmulatedProcess::makeSystemCall() to return
        process->setState((process->suspendCount() == 0) ? Process::State::Active : Process::State::Suspended);
        emulatedProcess->markSystemCallComplete();
    }
    else
    {   //  A native process is now "ready" to run
        process->setState((process->suspendCount() == 0) ? Process::State::Ready : Process::State::Suspended);
    }
}

void Scp::_handleWriteToOperatorSystemCall(WriteToOperatorSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    //  Locate the device driver that drives the "operator console" device
    if (_operatorsConsole == nullptr)
    {   //  OOPS! No "operator console"
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }
    if (!_deviceDrivers.contains(_operatorsConsole))
    {   //  OOPS! No device driver exists for "operator console"
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }
    DeviceDriver * operatorConsoleDriver = _deviceDrivers[_operatorsConsole];
    //  Is the device ready for I/O ?
    switch (_operatorsConsole->state())
    {
        case Device::State::Unknown:
        case Device::State::NonOperational:
            //  Can't perform I/O
            systemCall->setOutcome(ErrorCode::ERR_SUP);
            return;
        case Device::State::Ready:
            //  Instruct the driver to begin writing
            {
                ErrorCode err = operatorConsoleDriver->writeBlock(_operatorsConsole, systemCall->buffer, &_transferCompletionListener);
                if (err == ErrorCode::ERR_OK)
                {   //  I/O started - it will complete eventually
                    _ioInProgress.insert(_operatorsConsole, systemCall->process());
                    return;
                }
                if (err != ErrorCode::ERR_USE)
                {   //  OOPS! Couldn't!
                    systemCall->setOutcome(err);
                    return;
                }
            }
            //  Device is "ready", but perhaps another device on the same "controller" is
            //  already performing I/O, and "controller" can only handle one I/O at a time.
            //  Fall through - this will enqueue an "I/O request" for later completion
        case Device::State::Busy:
            //  Device is busy with another I/O - form an "I/O request" qnd queue it
            Q_ASSERT(false);
    }
}

void Scp::_handleReadFromOperatorSystemCall(ReadFromOperatorSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    //  Locate the device driver that drives the "operator console" device
    if (_operatorsConsole == nullptr)
    {   //  OOPS! No "operator console"
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }
    if (!_deviceDrivers.contains(_operatorsConsole))
    {   //  OOPS! No device driver exists for "operator console"
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }
    DeviceDriver * operatorConsoleDriver = _deviceDrivers[_operatorsConsole];
    //  Is the device ready for I/O ?
    switch (_operatorsConsole->state())
    {
        case Device::State::Unknown:
        case Device::State::NonOperational:
            //  Can't perform I/O
            systemCall->setOutcome(ErrorCode::ERR_SUP);
            return;
        case Device::State::Ready:
            //  The input must be space-padded to the length of the buffer!
            {
                static util::CharacterSet::Encoder * encoder = util::Cp037CharacterSet::getInstance()->createEncoder();
                static QByteArray spaceBytes;
                if (spaceBytes.size() == 0)
                {   //  Prepare ONCE
                    QChar ch;
                    encoder->encode(' ', spaceBytes);
                    Q_ASSERT(spaceBytes.size() == 1);
                }
                for (int i = 0; i < systemCall->buffer->size(); i++)
                {
                    systemCall->buffer->setAt(i, spaceBytes[0]);
                }
            }
            //  Instruct the driver to begin reading
            {
                ErrorCode err = operatorConsoleDriver->readBlock(_operatorsConsole, systemCall->buffer, &_transferCompletionListener);
                if (err == ErrorCode::ERR_OK)
                {   //  I/O started - it will complete eventually
                    _ioInProgress.insert(_operatorsConsole, systemCall->process());
                    return;
                }
                if (err != ErrorCode::ERR_USE)
                {   //  OOPS! Couldn't!
                    systemCall->setOutcome(err);
                    return;
                }
            }
            //  Device is "ready", but perhaps another device on the same "controller" is
            //  already performing I/O, and "controller" can only handle one I/O at a time.
            //  Fall through - this will enqueue an "I/O request" for later completion
        case Device::State::Busy:
            //  Device is busy with another I/O - form an "I/O request" qnd queue it
            Q_ASSERT(false);
    }
}

void Scp::_handleOpenFileSystemCall(OpenFileSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    //  Perform filename redirection
    QSet<QString> usedRedirectors;
    QString resolvedFileName = systemCall->fileName;
    usedRedirectors.insert(resolvedFileName);
    while (systemCall->process()->hasEnvironmentVariable(resolvedFileName))
    {
        int valueCount;
        if (systemCall->process()->getEnvironmentVariableValueCount(resolvedFileName, valueCount) != ErrorCode::ERR_OK ||
            valueCount != 1)
        {   //  OOPS! Stop resolving
            break;
        }
        QString value;
        if (systemCall->process()->getEnvironmentVariableValueScalar(resolvedFileName, value) != ErrorCode::ERR_OK)
        {   //  OOPS! Stop resolving
            break;
        }
        //  Perform 1 step of name resolution
        if (usedRedirectors.contains(value))
        {   //  A cycle!
            break;
        }
        usedRedirectors.insert(value);
        resolvedFileName = value;
    }

    //  Are we trying to open a device ?
    if (PhysicalDevice * physicalDevice = _objectManager.findPhysicalDeviceByName(resolvedFileName))
    {   //  Yes - locate the driver...
        DeviceDriver * deviceDriver = _deviceDrivers[physicalDevice];
        //  ...ask it to confirm the "open mode"...
        ErrorCode err = deviceDriver->validateOpen(systemCall->openFlags, systemCall->recordSize, systemCall->blockSize);
        if (err != ErrorCode::ERR_OK)
        {   //  OOPS! Can't open!
            systemCall->setOutcome(err);
            return;
        }
        //  ...allocate a yet-unused "handle"...
        ResourceHandle handle = 0;
        for (ResourceHandle h = 1; h != 0; h++)
        {   //  Loop until an overflow occurs
            if (!systemCall->process()->_openHandles.contains(h))
            {
                handle = h;
                break;
            }
        }
        if (handle == 0)
        {   //  OOPS! Too many open handles!
            systemCall->setOutcome(ErrorCode::ERR_LIM);
            return;
        }
        //  Create a new DeviceResource and insert it into the Process' handle table
        DeviceResource * deviceResource = new DeviceResource(this, systemCall->openFlags, systemCall->recordSize, systemCall->blockSize, physicalDevice);
        systemCall->process()->_openHandles.insert(handle, deviceResource);
        deviceResource->incrementOpenHandleCount();
        //  Store the hewly created "handle" and we're done
        systemCall->handle = handle;
        systemCall->setOutcome(ErrorCode::ERR_OK);
        return;
    }

    //  TODO finish the implementation
    systemCall->setOutcome(ErrorCode::ERR_NOF);
}

void Scp::_handleWriteToFileSystemCall(WriteToFileSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    //  Resolve I/O resource handle
    if (!systemCall->process()->_openHandles.contains(systemCall->handle))
    {   //  OOPS! Handle not open!
        systemCall->setOutcome(ErrorCode::ERR_PAR);
        return;
    }
    IResource * resource = systemCall->process()->_openHandles[systemCall->handle];
    Q_ASSERT(resource != nullptr);
    IIoResource * ioResource = dynamic_cast<IIoResource *>(resource);
    if (ioResource == nullptr)
    {   //  OOPS! Handle does not refer to I/O resource!
        systemCall->setOutcome(ErrorCode::ERR_PAR);
        return;
    }
    if ((ioResource->openFileFlags() & OpenFileFlags::DirectionMask) == OpenFileFlags::ReadOnly)
    {   //  OOPS! Can't write!
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }

    if (DeviceResource * deviceResource = dynamic_cast<DeviceResource*>(ioResource))
    {
        DeviceDriver * deviceDriver = _deviceDrivers[deviceResource->device()];
        switch (deviceResource->device()->state())
        {
            case Device::State::Unknown:
            case Device::State::NonOperational:
                systemCall->setOutcome(ErrorCode::ERR_SUP);
                break;
            case Device::State::Ready:
                //  Begin I/O
                ErrorCode err;
                if ((deviceResource->openFileFlags() & OpenFileFlags::RecordsMask) == OpenFileFlags::UndefinedRecords)
                {   //  Stream I/O
                    err = deviceDriver->write(deviceResource->device(), systemCall->buffer, &_transferCompletionListener);
                }
                else
                {   //  Record I/O
                    err = deviceDriver->writeBlock(deviceResource->device(), systemCall->buffer, &_transferCompletionListener);
                }
                //  Has I/O started ?
                if (err == ErrorCode::ERR_USE)
                {   //  Schedule IORP
                    Q_ASSERT(false);
                }
                else if (err != ErrorCode::ERR_OK)
                {   //  OOPS!
                    systemCall->setOutcome(ErrorCode::ERR_SUP);
                    break;
                }
                _ioInProgress.insert(deviceResource->device(), systemCall->process());
                break;
            case Device::State::Busy:
                Q_ASSERT(false);
                //  TODO schedule I/O request package
        }
    }
    else
    {   //  TODO finish the implementation
        systemCall->setOutcome(ErrorCode::ERR_SUP);
    }
}

void Scp::_handleReadFromFileSystemCall(ReadFromFileSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    //  Resolve I/O resource handle
    if (!systemCall->process()->_openHandles.contains(systemCall->handle))
    {   //  OOPS! Handle not open!
        systemCall->setOutcome(ErrorCode::ERR_PAR);
        return;
    }
    IResource * resource = systemCall->process()->_openHandles[systemCall->handle];
    Q_ASSERT(resource != nullptr);
    IIoResource * ioResource = dynamic_cast<IIoResource *>(resource);
    if (ioResource == nullptr)
    {   //  OOPS! Handle does not refer to I/O resource!
        systemCall->setOutcome(ErrorCode::ERR_PAR);
        return;
    }
    if ((ioResource->openFileFlags() & OpenFileFlags::DirectionMask) == OpenFileFlags::WriteOnly)
    {   //  OOPS! Can't read!
        systemCall->setOutcome(ErrorCode::ERR_SUP);
        return;
    }

    if (DeviceResource * deviceResource = dynamic_cast<DeviceResource*>(ioResource))
    {
        DeviceDriver * deviceDriver = _deviceDrivers[deviceResource->device()];
        switch (deviceResource->device()->state())
        {
            case Device::State::Unknown:
            case Device::State::NonOperational:
                systemCall->setOutcome(ErrorCode::ERR_SUP);
                break;
            case Device::State::Ready:
                //  Begin I/O
                ErrorCode err;
                if ((deviceResource->openFileFlags() & OpenFileFlags::RecordsMask) == OpenFileFlags::UndefinedRecords)
                {   //  Stream I/O
                    err = deviceDriver->read(deviceResource->device(), systemCall->buffer, &_transferCompletionListener);
                }
                else
                {   //  Record I/O
                    err = deviceDriver->readBlock(deviceResource->device(), systemCall->buffer, &_transferCompletionListener);
                }
                //  Has I/O started ?
                if (err == ErrorCode::ERR_USE)
                {   //  Schedule IORP
                    Q_ASSERT(false);
                }
                else if (err != ErrorCode::ERR_OK)
                {   //  OOPS!
                    systemCall->setOutcome(ErrorCode::ERR_SUP);
                    break;
                }
                _ioInProgress.insert(deviceResource->device(), systemCall->process());
                break;
            case Device::State::Busy:
                Q_ASSERT(false);
                //  TODO schedule I/O request package
        }
    }
    else
    {   //  TODO finish the implementation
        systemCall->setOutcome(ErrorCode::ERR_SUP);
    }
}

void Scp::_handleSetEnvironmentVariableValueSystemCall(SetEnvironmentVariableValueSystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    ErrorCode err = systemCall->targetProcess->setEnvironmentVariableValue(systemCall->name, systemCall->listValue);
    systemCall->setOutcome(err);
}

void Scp::_handleUnknownSystemCall(SystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(systemCall != nullptr);

    systemCall->setOutcome(ErrorCode::ERR_SVC);
}

void Scp::_handleTransferCompleteEvent(_TransferCompleteEvent * event)
{
    Q_ASSERT(QThread::currentThread() == _workerThread);
    Q_ASSERT(_ioInProgress.contains(event->_device));

    Process * process = _ioInProgress[event->_device];
    _ioInProgress.remove(event->_device);
    Q_ASSERT(process->state() == Process::State::Waiting ||
             process->state() == Process::State::SuspendedWaiting);
    //  The system call that led to I/O now has a known outcome
    Q_ASSERT(_systemCallsInProgress.contains(process));
    SystemCall * systemCall = _systemCallsInProgress[process];
    _systemCallsInProgress.remove(process);
    if (TransferDataSystemCall * transferDataSystemCall = dynamic_cast<TransferDataSystemCall*>(systemCall))
    {
        transferDataSystemCall->bytesTransferred = event->_bytesTransferred;
    }
    systemCall->setOutcome(event->_errorCode);
    //  The process is ready to continue running
    if (EmulatedProcess * emulatedProcess = dynamic_cast<EmulatedProcess*>(process))
    {   //  Must allow EmulatedProcess::makeSystemCall() to return
        process->setState((process->suspendCount() == 0) ? Process::State::Active : Process::State::Suspended);
        emulatedProcess->markSystemCallComplete();
    }
    else
    {   //  A native process is now ready to run
        process->setState((process->suspendCount() == 0) ? Process::State::Ready : Process::State::Suspended);
    }
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
    _scp->_objectManager.createEmulatedProcess(InitProcess::Application::getInstance()->mnemonic(),
                                               Process::Flags::System,
                                               nullptr,
                                               InitProcess::Application::getInstance(),
                                               initProcess);
    //  Go!
    initProcess->start();
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
        else if (_TransferCompleteEvent * transferCompleteEvent = dynamic_cast<_TransferCompleteEvent*>(event))
        {
            _scp->_handleTransferCompleteEvent(transferCompleteEvent);
        }
        delete event;
    }
    //  Stop all I/Os in progress
    for (Device * device : _scp->_deviceDrivers.keys())
    {
        DeviceDriver * deviceDriver = _scp->_deviceDrivers[device];
        deviceDriver->haltIo(device);
    }
    //  Stop all emulated processes
    for (EmulatedProcess * emulatedProcess : _scp->_objectManager.emulatedProcesses())
    {
        emulatedProcess->stop(ErrorCode::ERR_UNK);
    }
}

//  End of emuone-scp360/Scp.cpp
