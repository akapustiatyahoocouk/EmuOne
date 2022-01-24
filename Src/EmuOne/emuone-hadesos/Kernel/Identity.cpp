//
//  emuone-hadesos/Kernel/Identity.cpp
//
//  The hadesos::kernel::Identity class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos::kernel;

//////////
//  Construction/destruction
Identity::Identity(Kernel * kernel, ObjectId id, Identity * owner,
                   const QString & name, bool active)
    :   Object(kernel, id, owner),
        _name(name),
        _active(active)
{
    Q_ASSERT(kernel->isStateLocked());
}

Identity::~Identity()
{
    Q_ASSERT(kernel()->isStateLocked());
}

//  End of emuone-hadesos/Kernel/Identity.cpp
