//
//  emuone-util/ArchitectureManager.cpp - emuone::util::ArchitectureManager class implementation
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

struct ArchitectureManager::_Impl
{
    using Registry = QMap<QString, IArchitecture*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto ArchitectureManager::allArchitectures() -> Architectures
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return Architectures(result.cbegin(), result.cend());
}

bool ArchitectureManager::registerArchitecture(IArchitecture * architecture)
{
    Q_ASSERT(architecture != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = architecture->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return architecture == registered;
    }
    impl->registry[key] = architecture;
    return true;
}

bool ArchitectureManager::unregisterArchitecture(IArchitecture * architecture)
{
    Q_ASSERT(architecture != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = architecture->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (architecture == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
        }
        return false;
    }
    return false;
}

auto ArchitectureManager::findArchitecture(const QString & mnemonic) -> IArchitecture *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto ArchitectureManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-util/ArchitectureManager.cpp
