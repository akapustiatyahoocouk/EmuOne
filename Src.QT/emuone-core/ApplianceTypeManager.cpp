//
//  emuone-util/ApplianceTypeManager.cpp - emuone::util::ApplianceTypeManager class implementation
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

struct ApplianceTypeManager::_Impl
{
    using Registry = QMap<QString, IApplianceType*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto ApplianceTypeManager::allApplianceTypes() -> ApplianceTypes
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return ApplianceTypes(result.cbegin(), result.cend());
}

bool ApplianceTypeManager::registerApplianceType(IApplianceType * applianceType)
{
    Q_ASSERT(applianceType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = applianceType->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return applianceType == registered;
    }
    impl->registry[key] = applianceType;
    return true;
}

bool ApplianceTypeManager::unregisterApplianceType(IApplianceType * applianceType)
{
    Q_ASSERT(applianceType != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = applianceType->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (applianceType == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
        }
        return false;
    }
    return false;
}

auto ApplianceTypeManager::findApplianceType(const QString & mnemonic) -> IApplianceType *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto ApplianceTypeManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-util/ApplianceTypeManager.cpp
