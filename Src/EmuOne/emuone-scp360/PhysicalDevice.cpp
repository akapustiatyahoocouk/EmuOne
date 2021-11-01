//
//  emuone-scp360/PhysicalDevice.cpp
//
//  The scp360::PhysicalDevice class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;


//////////
//  Construction/destruction
PhysicalDevice::PhysicalDevice(Scp * scp, Id id, ibm360::Device * hardwareDevice, Flags flags, DeviceDriver * driver)
    :   Device(scp, id, _nameFromAddress(hardwareDevice->address()), flags, driver),
        _hardwareDevice(hardwareDevice)
{
}

PhysicalDevice::~PhysicalDevice()
{
}

//////////
//  Operations

//////////
//  Implementation helpers
QString PhysicalDevice::_nameFromAddress(uint16_t address)
{
    return "#" + ("000" + QString::number(address, 16)).right(3).toUpper();
}

//  End of emuone-scp360/PhysicalDevice.cpp
