//
//  emuone-hadesos/Kernel/Kernel.cpp
//
//  The hadesos::kernel::Kernel class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos::kernel;

//////////
//  Construction/destruction
Kernel::Kernel(IMemoryManager * memoryManager)
    :   _memoryManager(memoryManager)
{
    Q_ASSERT(_memoryManager != nullptr);

    util::Lock lock(_stateGuard);

    _createSystemIdentity();
}

Kernel::~Kernel()
{
}

//////////
//  Object table helpers
ObjectId Kernel::_generateUnusedObjectId()
{
    Q_ASSERT(_stateGuard.isLockedByCurrentThread());

    if (!_recycledObjectIds.isEmpty())
    {   //  Recycle an OID
        return _recycledObjectIds.dequeue();
    }
    //  Generatr a new, unused OID
    for (ObjectId id = MinObjectId; id < MaxObjectId; id++)
    {
        if (!_objects.contains(id))
        {
            return id;
        }
    }
    //  OOPS! Can't!
    return InvalidObjectId;
}

SystemIdentity * Kernel::_createSystemIdentity()
{
    Q_ASSERT(_stateGuard.isLockedByCurrentThread());

    ObjectId id = _generateUnusedObjectId();
    SystemIdentity * systemIdentity = new SystemIdentity(this, id);

    Q_ASSERT(!_objects.contains(id));
    _objects.insert(id, systemIdentity);

    Q_ASSERT(!_mapNamesToIdentities.contains(systemIdentity->name()));
    _mapNamesToIdentities.insert(systemIdentity->name(), systemIdentity);

    Q_ASSERT(_systemIdentity == nullptr);
    _systemIdentity = systemIdentity;

    return systemIdentity;
}

//  End of emuone-hadesos/Kernel/Kernel.cpp
