//
//  emuone-scp360/Object.cpp
//
//  The scp360::Object class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
Object::Object(Scp * scp, Id id)
    :   _scp(scp),
        _id(id)
{
    Q_ASSERT(_scp != nullptr);
}

Object::~Object()
{
}

//  End of emuone-scp360/Object.cpp
