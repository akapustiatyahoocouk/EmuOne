//
//  emuone-scp360/InitProcess.cpp
//
//  The scp360::InitProcess class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
InitProcess::InitProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
    :   EmulatedProcess(scp, id, name, flags, parent)
{
}

InitProcess::~InitProcess()
{
}

//////////
//  EmulatedProcess
EmulatedApplication * InitProcess::application() const
{
    return InitProcess::Application::getInstance();
}

ErrorCode InitProcess::run()
{
    //  Initialise all Devices that CAN be initialised (using their DeviceDrivers)
    _initialiseDevices();

    //  TODO Set default "TMPVOL" environment variable

    //  TODO move this to LOGIN process
    systemCalls.writeToOperator("*** SCP/360 VERSION " +
                                QString(__DATE__).toUpper() +
                                " " +
                                QString(__TIME__).toUpper() +
                                " ***");

    //  We need to create a "LOGIN" process for each "terminal"
    _createLoginProcesses();

    return ErrorCode::ERR_OK;
}

//////////
//  EmulatedApplication
EmulatedProcess * InitProcess::Application::createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
{
    return new InitProcess(scp, id, name, flags, parent);
}

//////////
//  InitProcess::Application
IMPLEMENT_SINGLETON(InitProcess::Application)
InitProcess::Application::Application() {}
InitProcess::Application::~Application() {}

QString InitProcess::Application::mnemonic() const
{
    return "INIT";
}

QString InitProcess::Application::displayName() const
{
    return "INIT";
}

//////////
//  InitProcess::_InitialiseDeviceCompletionListener
InitProcess::_InitialiseDeviceCompletionListener::_InitialiseDeviceCompletionListener(Device * device)
    :   device(device)
{
}

InitProcess::_InitialiseDeviceCompletionListener::~_InitialiseDeviceCompletionListener()
{
}

//////////
//  Implementation helpers
void InitProcess::_initialiseDevices()
{
    for (Device * device : scp()->_deviceDrivers.keys())
    {
        DeviceDriver * deviceDriver = scp()->_deviceDrivers[device];
        _InitialiseDeviceCompletionListener * ioCompletionListener = new _InitialiseDeviceCompletionListener(device);
        _initialiseDeviceCompletionListeners.append(ioCompletionListener);
        ErrorCode err = deviceDriver->initialiseDevice(device, ioCompletionListener);
        if (err != ErrorCode::ERR_OK)
        {   //  Notify the listener immediately
            ioCompletionListener->errorCode = err;
            ioCompletionListener->completed = true;
        }
    }
    //  ...and wait for all completion listeners to be notified if initialisation completion
    for (_InitialiseDeviceCompletionListener * ioCompletionListener : _initialiseDeviceCompletionListeners)
    {
        while (!ioCompletionListener->completed)
        {   //  Wait for a bit
            QThread::msleep(10);
        }
    }
    //  Report initialisation results to the system console
    for (_InitialiseDeviceCompletionListener * ioCompletionListener : _initialiseDeviceCompletionListeners)
    {
        if (ioCompletionListener->errorCode == ErrorCode::ERR_OK)
        {
            systemCalls.writeToOperator("Device " + ioCompletionListener->device->name() + " initialised successfully");
        }
        else
        {
            systemCalls.writeToOperator("Device " + ioCompletionListener->device->name() + " failed to initialise");    //  TODO why ?
        }
    }
    //  Clear initialisation results
    for (_InitialiseDeviceCompletionListener * ioCompletionListener : _initialiseDeviceCompletionListeners)
    {
        delete ioCompletionListener;
    }
    _initialiseDeviceCompletionListeners.clear();
}

void InitProcess::_createLoginProcesses()
{
    for (PhysicalDevice * physicalDevice : scp()->_objectManager.physicalDevices())
    {
        if (Scp::_isTerminal(physicalDevice->hardwareDevice()))
        {   //  This one! TODO use system calls to create LOGIN process
            QString loginProcessName = LoginProcess::Application::getInstance()->mnemonic() +
                                       ("000" + QString::number(physicalDevice->hardwareDevice()->address(), 16)).right(3).toUpper();
            EmulatedProcess * loginProcess;
            ErrorCode err = scp()->_objectManager.createEmulatedProcess(loginProcessName,
                                                                        Process::Flags::None,
                                                                        this,
                                                                        LoginProcess::Application::getInstance(),
                                                                        loginProcess);
            if (err != ErrorCode::ERR_OK)
            {   //  OOPS!
                systemCalls.writeToOperator("Count not create LOGIN process for terminal " + physicalDevice->name());
                continue;
            }
            //  Set up LOGIN-specific environment variables
            //  TODO CURVOL	The name of the “current” file system volume. Any attempt to refer to a named file without explicitly specifying the volume where it resides is assumed to refer to this volume.
            //  TODO TMPVOL	The volume where temporary/work files are created. For performance reasons this is usually a direct-access device.
            systemCalls.setEnvironmentVariableValue(loginProcess, "SYSIN", physicalDevice->name());
            systemCalls.setEnvironmentVariableValue(loginProcess, "SYSOUT", physicalDevice->name());
            //  Go!
            loginProcess->start();
        }
    }
}

//////////
//  InitProcess::_InitialiseDeviceCompletionListener
void InitProcess::_InitialiseDeviceCompletionListener::ioCompleted(Device * device, ErrorCode errorCode)
{
    Q_ASSERT(this->device == device);
    this->errorCode = errorCode;
    this->completed = true;
}

//  End of emuone-scp360/InitProcess.cpp
