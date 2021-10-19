//
//  emuone-scp360/Types.cpp
//
//  The SCP/360 basic types
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  OpenFileFlags
EMUONE_SCP360_EXPORT OpenFileFlags scp360::operator & (OpenFileFlags op1, OpenFileFlags op2)
{
    return OpenFileFlags(uint16_t(op1) & uint16_t(op2));
}

EMUONE_SCP360_EXPORT OpenFileFlags scp360::operator | (OpenFileFlags op1, OpenFileFlags op2)
{
    return OpenFileFlags(uint16_t(op1) | uint16_t(op2));
}

EMUONE_SCP360_EXPORT OpenFileFlags & scp360::operator &= (OpenFileFlags & op1, OpenFileFlags op2)
{
    op1 = op1 & op2;
    return op1;
}

EMUONE_SCP360_EXPORT OpenFileFlags & scp360::operator |= (OpenFileFlags & op1, OpenFileFlags op2)
{
    op1 = op1 | op2;
    return op1;
}

//  End of emuone-scp360/Types.cpp
