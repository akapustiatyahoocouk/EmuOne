//
//  emuone-core/VirtualMachineTypeManager.cpp - emuone::core::VirtualMachineTypeManager class implementation
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

struct VirtualMachineTypeManager::_Impl
{
    using Registry = QMap<QString, IVirtualMachineType*>;

    _Impl()
    {
        for (auto virtualMachineType : StandardVirtualMachineTypes::all())
        {
            QString key = virtualMachineType->mnemonic();
            Q_ASSERT(!registry.contains(key));
            registry[key] = virtualMachineType;
        }
    }

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto VirtualMachineTypeManager::all() -> VirtualMachineTypes
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return VirtualMachineTypes(result.cbegin(), result.cend());
}

bool VirtualMachineTypeManager::register(IVirtualMachineType * virtualMachineType)
{
    Q_ASSERT(virtualMachineType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = virtualMachineType->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return virtualMachineType == registered;
    }
    impl->registry[key] = virtualMachineType;
    return true;
}

bool VirtualMachineTypeManager::unregister(IVirtualMachineType * virtualMachineType)
{
    Q_ASSERT(virtualMachineType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = virtualMachineType->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (virtualMachineType == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto VirtualMachineTypeManager::find(const QString & mnemonic) -> IVirtualMachineType *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto VirtualMachineTypeManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-core/VirtualMachineTypeManager.cpp
