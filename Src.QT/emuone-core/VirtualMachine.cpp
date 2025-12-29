//
//  emuone-core/VirtualMachine.cpp - emuone::core::VirtualMachine class implementation
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
//  Construction/destruction
VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        IArchitecture * architecture,
        IVirtualMachineType * type,
        IVirtualMachineTemplate * createdFrom
    ) : _name(name.trimmed()),  //  be defensive!
        _location(QFileInfo(location).absoluteFilePath()),  //  be defensive!
        _architecture(architecture),
        _type(type),
        _createdFrom(createdFrom)
{
    Q_ASSERT(isValidName(_name)),
    Q_ASSERT(_architecture != nullptr);
    Q_ASSERT(_type != nullptr);
    Q_ASSERT( _createdFrom == nullptr ||
             (_createdFrom->architecture() == _architecture &&
              _createdFrom->virtualMachineType() == _type));
}

VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        IArchitecture * architecture,
        IVirtualMachineType * type
    ) : VirtualMachine(
            name,
            location,
            architecture,
            type,
            nullptr)
{
}

VirtualMachine::VirtualMachine(
        const QString & name,
        const QString & location,
        IVirtualMachineTemplate * createdFrom
    ) : VirtualMachine(
              name,
              location,
              createdFrom->architecture(),
              createdFrom->virtualMachineType(),
              createdFrom)
{
}

VirtualMachine::~VirtualMachine()
{
    //  TODO suspend/stop!
}

//////////
//  Operations (general)
QString VirtualMachine::name() const
{
    emuone::util::Lock _(_guard);
    return _name;
}

void VirtualMachine::setName(const QString & name)
{
    emuone::util::Lock _(_guard);

    Q_ASSERT(isValidName(name));
    if (isValidName(name))
    {   //  Be defensive in release mode
        _name = name;
    }
}

bool VirtualMachine::isValidName(const QString & name)
{
    return name.length() > 0 &&
           name.length() <= 128 &&
           name.trimmed().length() == name.length() &&
           std::all_of(
            name.cbegin(),
            name.cend(),
            [](auto c)
            {
                return c.unicode() >= 32 && c.unicode() != 127;
            });
}

QString VirtualMachine::location() const
{   //  No need to synchronize

    return _location;
}

QString VirtualMachine::savedStateFileName() const
{   //  No need to synchronize
    return _location + SavedStateExtension;
}

IArchitecture * VirtualMachine::architecture() const
{   //  No need to synchronize
    return _architecture;
}

auto VirtualMachine::type() const -> IVirtualMachineType *
{   //  No need to synchronize
    return _type;
}

auto VirtualMachine::createdFrom() const -> IVirtualMachineTemplate *
{   //  No need to synchronize
    return _createdFrom;
}

QIcon VirtualMachine::smallIcon() const
{
    return _architecture->smallIcon();
}

QIcon VirtualMachine::largeIcon() const
{
    return _architecture->largeIcon();
}

//////////
//  Operations (state control)
VirtualMachine::State VirtualMachine::state() const
{
    emuone::util::Lock _(_guard);
    return _state;
}
bool VirtualMachine::isStopped() const
{
    return state() == State::Stopped;
}

bool VirtualMachine::isRunning() const
{
    return state() == State::Running;
}

bool VirtualMachine::isSuspended() const
{
    return state() == State::Suspended;
}

//////////
//  Operations (persistency)
void VirtualMachine::save()
{
    emuone::util::Lock _(_guard);

    //  Only Stopped VMs can be saved
    if (_state != State::Stopped)
    {   //  OOPS!
        throw InvalidVirtualMachineStateException();
    }

    //  Create DOM document and root node
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    QDomElement rootElement = document.createElement("VirtualMachine");
    rootElement.setAttribute("Virtualization", "EmuOne");
    rootElement.setAttribute("FormatVersion", "1");
    rootElement.setAttribute("Name", _name);
    rootElement.setAttribute("Architecture", _architecture->mnemonic());
    rootElement.setAttribute("Type", _type->mnemonic());
    if (_createdFrom != nullptr)
    {
        rootElement.setAttribute("Template", _createdFrom->mnemonic());
    }
    document.appendChild(rootElement);

    //  Do the components
    //  TODO

    //  Save DOM & we're done
    QFile file(_location);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomVirtualMachineException(_location + ": " +  file.errorString());
    }
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();
}

auto VirtualMachine::load(const QString & location) -> VirtualMachine *
{
    //  Load XML DOM
    QDomDocument document;
    QFile file(location);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomVirtualMachineException(
            location + ": " + file.errorString());
    }
    if (!document.setContent(&file))
    {   //  OOPS!
        throw VirtualMachineCorruptException(location);
    }

    //  Validate root element
    QDomElement rootElement = document.documentElement();
    if (rootElement.isNull() ||
        rootElement.tagName() != "VirtualMachine" ||
        rootElement.attribute("Virtualization") != "EmuOne" ||
        rootElement.attribute("FormatVersion") != "1" ||
        !rootElement.hasAttribute("Name") ||
        !rootElement.hasAttribute("Architecture") ||
        !rootElement.hasAttribute("Type"))
    {   //  OOPS!
        throw VirtualMachineCorruptException(location);
    }
    QString name = rootElement.attribute("Name");;
    auto architecture =
        ArchitectureManager::find(
            rootElement.attribute("Architecture"));
    //  TODO a separate "unsupported VM architecture" exception
    auto type =
        VirtualMachineTypeManager::find(
            rootElement.attribute("Type"));
    //  TODO a separate "unsupported VM type" exception
    auto createdFrom =
        VirtualMachineTemplateManager::find(
            rootElement.attribute("Template"));
    if (!isValidName(name) ||
        architecture == nullptr ||
        type == nullptr ||
        (createdFrom != nullptr &&
         (createdFrom->architecture() != architecture ||
          createdFrom->virtualMachineType() != type)))
    {   //  OOPS!
        throw VirtualMachineCorruptException(location);
    }

    //  Create new, empty VM (delete on load exception)
    std::unique_ptr<VirtualMachine> vm
    { new VirtualMachine(
        name,
        location,
        architecture,
        type,
        createdFrom) };

    //  TODO components

    //  All done
    return vm.release();
}

//  End of emuone-core/VirtualMachine.cpp
