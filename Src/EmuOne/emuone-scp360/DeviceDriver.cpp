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
DeviceDriver::DeviceDriver(ibm360::Device * device)
    :   _device(device)
{
    Q_ASSERT(_device != nullptr);
}

DeviceDriver::~DeviceDriver()
{
}

//////////
//  Operations
DeviceDriver * DeviceDriver::create(ibm360::Device * device)
{
    Q_ASSERT(device != nullptr);

    if (ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(device))
    {
        return new Ibm2741Driver(ibm2741);
    }

    throw core::VirtualApplianceException("Cannot instantiate device drived for " + device->type()->displayName());
}

//  End of emuone-scp360/DeviceDriver.cpp
