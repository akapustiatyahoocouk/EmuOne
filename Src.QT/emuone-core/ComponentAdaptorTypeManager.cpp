//
//  emuone-core/ComponentAdaptorTypeManager.cpp - emuone::core::ComponentAdaptorTypeManager class implementation
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

struct ComponentAdaptorTypeManager::_Impl
{
    using Registry = QMap<QString, IComponentAdaptorType*>;

    emuone::util::Mutex guard;
    Registry            registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto ComponentAdaptorTypeManager::all() -> ComponentAdaptorTypes
{
    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto result = impl->registry.values();
    return ComponentAdaptorTypes(result.cbegin(), result.cend());
}

bool ComponentAdaptorTypeManager::register(IComponentAdaptorType * componentAdaptorType)
{
    Q_ASSERT(componentAdaptorType != nullptr);

    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto key = componentAdaptorType->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return componentAdaptorType == registered;
    }
    impl->registry[key] = componentAdaptorType;
    return true;
}

bool ComponentAdaptorTypeManager::unregister(IComponentAdaptorType * componentAdaptorType)
{
    Q_ASSERT(componentAdaptorType != nullptr);

    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto key = componentAdaptorType->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (componentAdaptorType == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto ComponentAdaptorTypeManager::find(const QString & mnemonic) -> IComponentAdaptorType *
{
    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

auto ComponentAdaptorTypeManager::find(
        IArchitecture * architecture,
        IComponentType * componentType
    ) -> IComponentAdaptorType *
{
    Q_ASSERT(architecture != nullptr);
    Q_ASSERT(componentType != nullptr);

    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    for (auto cat : impl->registry.values())
    {
        if (cat->architecture() == architecture &&
            cat->componentType() == componentType)
        {
            return cat;
        }
    }
    return nullptr;
}

//////////
//  Implementation
auto ComponentAdaptorTypeManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-core/ComponentAdaptorTypeManager.cpp
