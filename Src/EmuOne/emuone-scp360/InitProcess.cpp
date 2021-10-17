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
    return InitProcess::InitApplication::getInstance();
}

uint16_t InitProcess::run()
{
    //  Initialise all Devices that CAN be initialised (using their DeviceDrivers)
    _initialiseDevices();



    return 0;
}

//////////
//  InitProcess::InitApplication
IMPLEMENT_SINGLETON(InitProcess::InitApplication)
InitProcess::InitApplication::InitApplication() {}
InitProcess::InitApplication::~InitApplication() {}

QString InitProcess::InitApplication::mnemonic() const
{
    return "INIT";
}

QString InitProcess::InitApplication::displayName() const
{
    return "INIT";
}


//////////
//  EmulatedApplication
EmulatedProcess * InitProcess::InitApplication::createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
{
    return new InitProcess(scp, id, name, flags, parent);
}

//////////
//  Construction/destruction
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
            writeToOperator("Device " + ioCompletionListener->device->name() + " initialised successfully");
        }
        else
        {
            writeToOperator("Device " + ioCompletionListener->device->name() + " failed to initialise");    //  TODO why ?
        }
    }
    //  Clear initialisation results
    for (_InitialiseDeviceCompletionListener * ioCompletionListener : _initialiseDeviceCompletionListeners)
    {
        delete ioCompletionListener;
    }
    _initialiseDeviceCompletionListeners.clear();
}

//////////
//  InitProcess::_InitialiseDeviceCompletionListener
void InitProcess::_InitialiseDeviceCompletionListener::ioCompleted(Device * device, ErrorCode errorCode)
{
    Q_ASSERT(this->device == device);
    this->errorCode = errorCode;
}

//  End of emuone-scp360/InitProcess.cpp
