//
//  emuone-hades/SystemProcessManager.cpp - emuone::hades::SystemProcessManager class implementation
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
#include "emuone-hades/API.hpp"
using namespace emuone::hades;

struct SystemProcessManager::_Impl
{
    using Registry = QMap<QString, ISystemProcess*>;

    emuone::util::Mutex guard;
    Registry            registry;   //  mnemonic -> VAT

    _Impl()
    {
#define EMUONE_REGISTER_SYSTEM_PROCESS(P)   \
        registry[systemprocesses::P::instance()->mnemonic()] = systemprocesses::P::instance();
        EMUONE_REGISTER_SYSTEM_PROCESS(Init)
    }
};

//////////
//  Operations
auto SystemProcessManager::all() -> SystemProcesses
{
    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto result = impl->registry.values();
    return SystemProcesses(result.cbegin(), result.cend());
}

bool SystemProcessManager::register(ISystemProcess * systemProcess)
{
    Q_ASSERT(systemProcess != nullptr);

    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto key = systemProcess->mnemonic();
    if (impl->registry.contains(key))
    {   //  Repeated registration is a kind of "success"
        auto registered = impl->registry[key];
        return systemProcess == registered;
    }
    impl->registry[key] = systemProcess;
    return true;
}

bool SystemProcessManager::unregister(ISystemProcess * systemProcess)
{
    Q_ASSERT(systemProcess != nullptr);

    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    auto key = systemProcess->mnemonic();
    if (impl->registry.contains(key))
    {
        auto registered = impl->registry[key];
        if (systemProcess == registered)
        {   //  We're not trying to un-register an impersonator
            impl->registry.remove(key);
            return true;
        }
    }
    return false;
}

auto SystemProcessManager::find(const QString & mnemonic) -> ISystemProcess *
{
    _Impl * impl = _impl();
    emuone::util::Lock _(impl->guard);

    return impl->registry.value(mnemonic, nullptr);
}

//////////
//  Implementation
auto SystemProcessManager::_impl() -> _Impl *
{
    static _Impl impl;
    return &impl;
}

//  End of emuone-hades/SystemProcessManager.cpp
