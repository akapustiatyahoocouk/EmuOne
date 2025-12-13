//
//  emuone-util/ComponentTypeManager.cpp - emuone::util::ComponentTypeManager class implementation
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

struct ComponentTypeManager::_Impl
{
    using Registry = QMap<QString, IComponentType*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto ComponentTypeManager::allComponentTypes() -> ComponentTypes
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return ComponentTypes(result.cbegin(), result.cend());
}

bool ComponentTypeManager::registerComponentType(IComponentType * componentType)
{
    Q_ASSERT(componentType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = componentType->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return componentType == registered;
    }
    impl->registry[key] = componentType;
    return true;
}

bool ComponentTypeManager::unregisterComponentType(IComponentType * componentType)
{
    Q_ASSERT(componentType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = componentType->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (componentType == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
        }
        return false;
    }
    return false;
}

auto ComponentTypeManager::findComponentType(const QString & mnemonic) -> IComponentType *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto ComponentTypeManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-util/ComponentTypeManager.cpp
