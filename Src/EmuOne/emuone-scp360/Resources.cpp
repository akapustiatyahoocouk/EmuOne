//
//  emuone-scp360/Resources.cpp
//
//  The scp360::Resource classes implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Resource
bool IResource::incrementOpenHandleCount()
{
    uint32_t newOpenHandleCount = _openHandleCount + 1;
    if (newOpenHandleCount > _openHandleCount)
    {
        _openHandleCount = newOpenHandleCount;
        return true;
    }
    return false;   //  overflow!
}

void IResource::decrementOpenHandleCount()
{
    Q_ASSERT(_openHandleCount != 0);
    _openHandleCount--;
}

//  End of emuone-scp360/Resources.cpp
