//
//  emuone-ibm360/Features.cpp
//
//  ibm360::Features implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Helper functions
EMUONE_IBM360_EXPORT Features ibm360::normalise(Features features)
{
    Features result = features;
    if ((result & Features::FetchProtection) != Features::None)
    {
        result |= Features::StoreProtection;
    }
    return result;
}

//  End of emuone-360/Features.cpp
