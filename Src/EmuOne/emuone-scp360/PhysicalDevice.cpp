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
bool PhysicalDevice::isValidName(const QString & name)
{
    return name.length() == 4 &&
           name[0] == '#' &&
           (name[1] >= '0' && name[0] <= '6') &&
           ((name[2] >= '0' && name[2] <= '9') || (name[2] >= 'A' && name[2] <= 'F')) &&
           ((name[3] >= '0' && name[3] <= '9') || (name[3] >= 'A' && name[3] <= 'F'));
}

//////////
//  Implementation helpers
QString PhysicalDevice::_nameFromAddress(uint16_t address)
{
    return "#" + ("000" + QString::number(address, 16)).right(3).toUpper();
}

//  End of emuone-scp360/PhysicalDevice.cpp
