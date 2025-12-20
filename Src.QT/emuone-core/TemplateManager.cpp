//
//  emuone-core/TemplateManager.cpp - emuone::util::TemplateManager class implementation
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

struct TemplateManager::_Impl
{
    using Registry = QMap<QString, ITemplate*>;

    QMutex      guard;
    Registry    registry;   //  mnemonic -> VAT
};

//////////
//  Operations
auto TemplateManager::allTemplates() -> Templates
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto result = impl->registry.values();
    return Templates(result.cbegin(), result.cend());
}

bool TemplateManager::registerTemplate(ITemplate * vaTemplate)
{
    Q_ASSERT(vaTemplate != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = vaTemplate->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return vaTemplate == registered;
    }
    impl->registry[key] = vaTemplate;
    return true;
}

bool TemplateManager::unregisterTemplate(ITemplate * vaTemplate)
{
    Q_ASSERT(vaTemplate != nullptr);

    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    auto key = vaTemplate->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (vaTemplate == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto TemplateManager::findTemplate(const QString & mnemonic) -> ITemplate *
{
    _Impl * impl = _impl();
    QMutexLocker _(&impl->guard);

    return impl->registry.contains(mnemonic) ?
               impl->registry[mnemonic] :
               nullptr;
}

//////////
//  Implementation
auto TemplateManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-core/TemplateManager.cpp
