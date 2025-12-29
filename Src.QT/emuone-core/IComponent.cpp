//
//  emuone-core/IComponent.cpp - emuone::core::IComponent class implementation
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

//////////
//  emuone::util::IStockObject
QIcon IComponent::smallIcon() const
{
    return type()->smallIcon();
}

QIcon IComponent::largeIcon() const
{
    return type()->largeIcon();
}

//////////
//  Operations
VirtualMachine * IComponent::owner() const
{   //  Guarded, as Components can be added/removed to/from VMs
    emuone::util::Lock _(guard);
    return _owner;
}

void IComponent::saveConfiguration(QDomElement & element) const
{
    Q_ASSERT(!element.isNull());
    element.setAttribute("Type", type()->mnemonic());
}

void IComponent::restoreConfiguration(const QDomElement & element)
{
    Q_ASSERT(!element.isNull());
    Q_ASSERT(element.attribute("Type") == type()->mnemonic());
}

//  End of emuone-core/IComponent.cpp
