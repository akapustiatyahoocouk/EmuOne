//
//  emuone-scp360/DeviceDriver.cpp
//
//  The scp360::DeviceDriver class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
DeviceDriver::DeviceDriver()
{
}

DeviceDriver::~DeviceDriver()
{
}

//////////
//  Operations
DeviceDriver * DeviceDriver::createHardwareDeviceDriver(ibm360::Device * hardwareDevice)
{
    Q_ASSERT(hardwareDevice != nullptr);

    if (dynamic_cast<ibm360::Ibm2741*>(hardwareDevice))
    {
        return new Ibm2741Driver();
    }
    throw core::VirtualApplianceException("No device driver for " + hardwareDevice->type()->displayName());
}

//  End of emuone-scp360/DeviceDriver.cpp
