//
//  emuone-scp360/SystemCalls.cpp
//
//  The emuone-scp360 system calls
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Helper functions
EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags scp360::operator & (OpenFileSystemCall::OpenFlags op1, OpenFileSystemCall::OpenFlags op2)
{
    return OpenFileSystemCall::OpenFlags(uint16_t(op1) & uint16_t(op2));
}

EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags scp360::operator | (OpenFileSystemCall::OpenFlags op1, OpenFileSystemCall::OpenFlags op2)
{
    return OpenFileSystemCall::OpenFlags(uint16_t(op1) | uint16_t(op2));
}

EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags & scp360::operator &= (OpenFileSystemCall::OpenFlags & op1, OpenFileSystemCall::OpenFlags op2)
{
    op1 = op1 & op2;
    return op1;
}

EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags & scp360::operator |= (OpenFileSystemCall::OpenFlags & op1, OpenFileSystemCall::OpenFlags op2)
{
    op1 = op1 | op2;
    return op1;
}

//  End of emuone-scp360/SystemCalls.cpp
