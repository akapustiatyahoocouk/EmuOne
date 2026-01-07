//
//  emuone-hades/systemprocesses/Init.cpp - emuone::hades::systemprocesses::Init class implementation
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
EMUONE_IMPLEMENT_SINGLETON(Init)
Init::Init() {}
Init::~Init() {}

//////////
//  ISystemProcess
QString Init::virtualPath() const
{
    return VirtuaPath;
}

QString Init::processName() const
{
    return ProcessName;
}

auto Init::createRunner() -> Runner *
{
    return new _Runner();
}

//////////
//  Init::_Runner
uint32_t Init::_Runner::run()
{
    systemCalls.kwrite("HADES Kernel " + Component::instance()->version().toString() + "\n");

    //  We'll need SystemIdentity and a
    //  NativeExecutionEnvironment
    _systemIdentity = _nativeThread->kernel->_systemIdentity;
    _executionEnvironment =
        dynamic_cast<kernel::NativeExecutionEnvironment*>(
            _nativeThread->process->executionEnvironment);
    Q_ASSERT(_executionEnvironment != nullptr);

    //  Respect signals from the Kernel
    systemCalls.signal(
        kernel::K_SIGTERM,
        [this](int sig) { this->_sighandler(sig); });

    //  Start the "device manager" process.
    //  NOTE, that it will be some time before
    //  the standard messaging mechanisms could
    //  be used
    {   //  We're manipulating Kernel data structures directly!
        emuone::util::Lock _(_nativeThread->kernel->kernelGuard);

        kernel::NativeProcess * deviceManagerProcess = nullptr;
        _nativeThread->kernel->createNativeProcess(
            _systemIdentity,
            _executionEnvironment,
            _nativeThread->process,
            kernel::PriorityClass::Normal,
            systemprocesses::DeviceManager::ProcessName,  //  name
            systemprocesses::DeviceManager::VirtuaPath,   //  command
            systemprocesses::DeviceManager::VirtuaPath,   //  command line
            systemprocesses::DeviceManager::CurrentDirectory,
            deviceManagerProcess);
        Q_ASSERT(deviceManagerProcess != nullptr &&
                 deviceManagerProcess->state == kernel::Process::State::Created);

        kernel::NativeThread * deviceManagerThread = nullptr;
        _nativeThread->kernel->createNativeThread(
            _systemIdentity,
            deviceManagerProcess,
            deviceManagerProcess->priorityClass,
            deviceManagerProcess->name,
            systemprocesses::DeviceManager::instance()->createRunner(),
            deviceManagerThread);
        Q_ASSERT(deviceManagerThread != nullptr &&
                 deviceManagerThread->state == kernel::Thread::State::Created);

        _nativeThread->kernel->startProcess(deviceManagerProcess);
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
void Init::_Runner::_sighandler(int /*sig*/)
{
    throw uint32_t(0);  //  TODO use exit(0) system call
}

//  End of emuone-hades/systemprocesses/Init.cpp
