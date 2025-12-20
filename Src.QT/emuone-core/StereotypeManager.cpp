//
//  emuone-core/StereotypeManager.cpp - emuone::core::StereotypeManager class implementation
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

struct StereotypeManager::_Impl
{
    using Registry = QMap<QString, IStereotype*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto StereotypeManager::allStereotypes() -> Stereotypes
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return Stereotypes(result.cbegin(), result.cend());
}

bool StereotypeManager::registerStereotype(IStereotype * stereotype)
{
    Q_ASSERT(stereotype != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = stereotype->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return stereotype == registered;
    }
    impl->registry[key] = stereotype;
    return true;
}

bool StereotypeManager::unregisterStereotype(IStereotype * stereotype)
{
    Q_ASSERT(stereotype != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = stereotype->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (stereotype == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto StereotypeManager::findStereotype(const QString & mnemonic) -> IStereotype *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto StereotypeManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-core/StereotypeManager.cpp
