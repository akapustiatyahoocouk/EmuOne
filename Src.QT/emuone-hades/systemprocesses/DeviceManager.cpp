//
//  emuone-hades/systemprocesses/DeviceManager.cpp - emuone::hades::systemprocesses::DeviceManager class implementation
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
    using namespace emuone::hades::systemprocesses;

//////////
//  Singleton
EMUONE_IMPLEMENT_SINGLETON(DeviceManager)
DeviceManager::DeviceManager() {}
DeviceManager::~DeviceManager() {}

//////////
//  ISystemProcess
QString DeviceManager::virtualPath() const
{
    return VirtuaPath;
}

QString DeviceManager::processName() const
{
    return ProcessName;
}

auto DeviceManager::createRunner() -> Runner *
{
    return new _Runner();
}

//////////
//  DeviceManager::_Runner
uint32_t DeviceManager::_Runner::run()
{
    //  Respect signals from the Kernel
    systemCalls.signal(
        kernel::K_SIGTERM,
        [this](int sig) { this->_sighandler(sig); });

    //  Need to determine which devicea to aupport.
    //  These are a) all native devices in the VM and
    //  b) all adaptors (NOT adapted devices!) there
    for (auto component : _nativeThread->kernel->owner()->nativeComponents())
    {
        if (auto device =
            dynamic_cast<emuone::core::IDevice*>(component))
        {
        }
    }
    for (auto component : _nativeThread->kernel->owner()->adaptedComponents())
    {
        if (auto device =
            dynamic_cast<emuone::core::IDevice*>(component))
        {
            auto adaptor = _nativeThread->kernel->owner()->findAdaptor(device);
        }
    }

    //  TODO implement
    for(; ; )
    {
        systemCalls.yield();
        QThread::msleep(500);
    }

    return 0;
}

//////////
//  Implementation helpers
void DeviceManager::_Runner::_sighandler(int /*sig*/)
{
    throw uint32_t(0);  //  TODO use exit(0) system call
}

//  End of emuone-hades/systemprocesses/Init.cpp
