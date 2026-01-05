//
//  emuone-hades/kernel/NativeProcess.cpp - emuone::hades::kernel::NativeProcess class implementation
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
    using namespace emuone::hades::kernel;

//////////
//  Construction/destruction
NativeProcess::NativeProcess(
        Kernel * kernel, const Oid & oid, Identity * owner,
        NativeExecutionEnvironment * executionEnvironment,
        Process * parent,
        PriorityClass priorityClass,
        const QString & name,
        const QString & command,
        const QString & commandLine,
        const QString & currentDirectory
    ) : Process(
            kernel, oid, owner,
            executionEnvironment,
            parent,
            priorityClass,
            name,
            command,
            commandLine,
            currentDirectory)
{
    //  Set up standard signal dispositions
    for (int i = 0; i < K_NSIG; i++)
    {
        defaultSignalDispositions[i] = K_SIG_IGN;
    }
    defaultSignalDispositions[K_SIGHUP]   = K_SIG_TRM;
    defaultSignalDispositions[K_SIGINT]   = K_SIG_TRM;
    defaultSignalDispositions[K_SIGQUIT]  = K_SIG_TRD;
    defaultSignalDispositions[K_SIGILL]   = K_SIG_TRD;
    defaultSignalDispositions[K_SIGABRT]  = K_SIG_TRD;
    defaultSignalDispositions[K_SIGFPE]   = K_SIG_TRD;
    defaultSignalDispositions[K_SIGKILL]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGUSR1]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGSEGV]  = K_SIG_TRD;
    defaultSignalDispositions[K_SIGUSR2]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGPIPE]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGALRM]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGTERM]  = K_SIG_TRM;
    defaultSignalDispositions[K_SIGCHLD]  = K_SIG_IGN;
    defaultSignalDispositions[K_SIGCONT]  = K_SIG_CNT;
    defaultSignalDispositions[K_SIGSTOP]  = K_SIG_STP;
    defaultSignalDispositions[K_SIGTSTP]  = K_SIG_STP;
    defaultSignalDispositions[K_SIGBREAK] = K_SIG_TRM;
    for (int i = 0; i < K_NSIG; i++)
    {
        signalDispositions[i] = defaultSignalDispositions[i];
    }
}

NativeProcess::~NativeProcess()
{
}

//  End of emuone-hades/kernel/NativeProcess.cpp
