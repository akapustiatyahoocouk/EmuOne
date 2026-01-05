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
    return "SYSTEM:/bin/init";
}

QString Init::processName() const
{
    return "init";
}

auto Init::createRunner() -> Runner *
{
    return new _Runner();
}

//////////
//  Init::_Runner
uint32_t Init::_Runner::run()
{
    //  Respect signals from the Kernel
    //  TODO implement
    for(; ; )
    {
        systemCalls.yield();
    }

    return 0;
}

//  End of emuone-hades/systemprocesses/Init.cpp
