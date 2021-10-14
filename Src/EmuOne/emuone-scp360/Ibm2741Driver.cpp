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
Ibm2741Driver::Ibm2741Driver(ibm360::Ibm2741 * ibm2741)
    :   DeviceDriver(ibm2741),
        _ibm2741(ibm2741)
{
}

Ibm2741Driver::~Ibm2741Driver()
{
}

//  End of emuone-scp360/Ibm2741Driver.cpp
