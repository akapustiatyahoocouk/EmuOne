//
//  emuone-hadesos/Kernel/Object.cpp
//
//  The hadesos::kernel::Object class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos::kernel;

//////////
//  Construction/destruction
Object::Object(Kernel * kernel, ObjectId id,Identity * owner)
    :   _kernel(kernel),
        _id(id),
        _owner(owner)
{
    Q_ASSERT(_kernel != nullptr);
    Q_ASSERT(_id >= MinObjectId && _id <= MaxObjectId);
    Q_ASSERT(_owner != nullptr);
}

Object::~Object()
{
}

//  End of emuone-hadesos/Kernel/Object.cpp
