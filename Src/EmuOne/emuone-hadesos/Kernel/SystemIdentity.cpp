//
//  emuone-hadesos/Kernel/SystemIdentity.cpp
//
//  The hadesos::kernel::SystemIdentity class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos::kernel;

//////////
//  Construction/destruction
SystemIdentity::SystemIdentity(Kernel * kernel, ObjectId id)
    :   Identity(kernel, id, this, "SYSTEM", true)
{
}

SystemIdentity::~SystemIdentity()
{
}

//  End of emuone-hadesos/Kernel/SystemIdentity.cpp
