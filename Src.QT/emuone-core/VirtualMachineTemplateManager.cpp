//
//  emuone-core/VirtualMachineTemplateManager.cpp - emuone::core::VirtualMachineTemplateManager class implementation
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

struct VirtualMachineTemplateManager::_Impl
{
    using Registry = QMap<QString, IVirtualMachineTemplate*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto VirtualMachineTemplateManager::all() -> VirtualMachineTemplates
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return VirtualMachineTemplates(result.cbegin(), result.cend());
}

bool VirtualMachineTemplateManager::register(IVirtualMachineTemplate * virtualMachineTemplate)
{
    Q_ASSERT(virtualMachineTemplate != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = virtualMachineTemplate->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return virtualMachineTemplate == registered;
    }
    impl->registry[key] = virtualMachineTemplate;
    return true;
}

bool VirtualMachineTemplateManager::unregister(IVirtualMachineTemplate * virtualMachineTemplate)
{
    Q_ASSERT(virtualMachineTemplate != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = virtualMachineTemplate->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (virtualMachineTemplate == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto VirtualMachineTemplateManager::find(const QString & mnemonic) -> IVirtualMachineTemplate *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto VirtualMachineTemplateManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-core/VirtualMachineTemplateManager.cpp
