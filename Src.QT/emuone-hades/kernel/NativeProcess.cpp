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
}

NativeProcess::~NativeProcess()
{
}

//  End of emuone-hades/kernel/NativeProcess.cpp
