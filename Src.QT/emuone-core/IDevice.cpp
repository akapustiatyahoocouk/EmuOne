//
//  emuone-core/IDevice.cpp - emuone::core::IDevice class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-core/API.hpp"
using namespace emuone::core;

//////////
//  Operations
auto IDevice::responseListeners(
        ) const -> ResponseListeners
{
    emuone::util::Lock _(_responseListenersGuard);

    ResponseListeners result(_responseListeners.size());
    for (int i = 0; i < _responseListeners.size(); i++)
    {
        result[i] = _responseListeners[i];
    }
    return result;
}

void IDevice::addResponseListener(
        IResponseListener * listener
    )
{
    Q_ASSERT(listener != nullptr);

    emuone::util::Lock _(_responseListenersGuard);
    if (!_responseListeners.contains(listener))
    {
        _responseListeners.append(listener);
    }
}

void IDevice::removeResponseListener(
        IResponseListener * listener
    )
{
    Q_ASSERT(listener != nullptr);

    emuone::util::Lock _(_responseListenersGuard);
    _responseListeners.removeOne(listener);
}

void IDevice::clearResponseListeners()
{
    emuone::util::Lock _(_responseListenersGuard);
    _responseListeners.clear();
}

void IDevice::dispatchResponse(Response * response)
{
    //  Can we avoid copying the listeners list onto the heap ?
    _responseListenersGuard.grab();
    if (qsizetype numListeners = _responseListeners.size();
        numListeners <= _MaxStaticListeners)
    {   //  Yes! Copy locally...
        IResponseListener * listeners[_MaxStaticListeners]; //  on the stack!
        for (int i = 0; i < _responseListeners.size(); i++)
        {
            listeners[i] = _responseListeners[i];
        }
        //  ...then unlock & dispatch
        _responseListenersGuard.release();
        for (int i = 0; i < numListeners; i++)
        {
            listeners[i]->onResponse(response); //  TODO may throw
        }
    }
    else
    {   //  Need a copy of the listeners list
        _responseListenersGuard.release();
        auto ll = responseListeners();  //  A nonshared copy
        for (int i = 0; i < ll.size(); i++)
        {
            ll[i]->onResponse(response); //  TODO may throw
        }
    }
}

//  End of emuone-core/IDevice.cpp
