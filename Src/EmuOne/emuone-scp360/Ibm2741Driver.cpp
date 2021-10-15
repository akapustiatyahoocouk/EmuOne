//
//  emuone-scp360/Ibm2741Driver.cpp
//
//  The scp360::Ibm2741Driver class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
Ibm2741Driver::Ibm2741Driver()
{
}

Ibm2741Driver::~Ibm2741Driver()
{
}

//////////
//  DeviceDriver
scp360::Device::Flags Ibm2741Driver::deviceFlags() const
{
    return Device::Flags::Initialise |
           Device::Flags::Deinitialise;
}

ErrorCode Ibm2741Driver::beginInitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(ioCompletionListener != nullptr);

    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }
    ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    return ErrorCode::ERR_SUP;
}

ErrorCode Ibm2741Driver::beginDeinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(ioCompletionListener != nullptr);

    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }
    ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    return ErrorCode::ERR_SUP;
}

//  End of emuone-scp360/Ibm2741Driver.cpp
