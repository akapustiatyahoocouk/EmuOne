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

bool VirtualMachine::isPersistable() const
{
    emuone::util::Lock _(_guard);

    return
        std::all_of(
            _nativeComponents.cbegin(),
            _nativeComponents.cend(),
            [](auto c) { return c->type()->isPersistable(); }) &&
        std::all_of(
            _adaptedComponents.cbegin(),
            _adaptedComponents.cend(),
            [](auto c) { return c->type()->isPersistable(); }) &&
        std::all_of(
            _adaptors.cbegin(),
            _adaptors.cend(),
               [](auto a) { return a->type()->isPersistable(); });
}

//////////
//  Operations (configuration)
Components VirtualMachine::components() const
{
    emuone::util::Lock _(_guard);
    return _nativeComponents + _adaptedComponents;
}

Components VirtualMachine::nativeComponents() const
{
    emuone::util::Lock _(_guard);
    return Components(_nativeComponents);   //  shallow clone
}

Components VirtualMachine::adaptedComponents() const
{
    emuone::util::Lock _(_guard);
    return Components(_adaptedComponents);   //  shallow clone
}

auto VirtualMachine::findAdaptor(IComponent * component) const -> IComponentAdaptor *
{
    emuone::util::Lock _(_guard);

    for (auto a : _adaptors)
    {
        if (a->component() == component)
        {
            Q_ASSERT(a->owner() == this && a->component()->owner() == this);
            return a;
        }
    }
    return nullptr;
}

void VirtualMachine::addComponent(
        IComponent * component,
        IComponentAdaptorType * componentAdaptorType
    )
{
    Q_ASSERT(component != nullptr);
    //  Synchronize IN ORDER, so there can't be a deadlock
    emuone::util::Lock _1(_guard);
    emuone::util::Lock _2(component->guard);

    Q_ASSERT(!component->isBound());    //  TODO throw instead
    //  Already added ?
    if (component->owner() == this)
    {   //  Nothing to do
        Q_ASSERT(_nativeComponents.contains(component) ||
                 _adaptedComponents.contains(component));
        return;
    }
    //  Can we add as native ?
    if (component->type()->isCompatibleWith(_architecture) &&
        component->type()->isCompatibleWith(_type))
    {   //  Yes
        _nativeComponents.insert(component);
        component->_owner = this;
        return;
    }
    //  Can we add using the specified comonent adaptor type ?
    if (componentAdaptorType != nullptr &&
        componentAdaptorType->architecture() == _architecture &&
        componentAdaptorType->componentType() == component->type() &&
        component->type()->isCompatibleWith(_type))
    {   //  Yes!
        _adaptedComponents.insert(component);
        component->_owner = this;
        auto adaptor = componentAdaptorType->createAdaptor(this, component);
        _adaptors.insert(adaptor);
        return;
    }
    //  Can we add using ANY comonent adaptor type ?
    for (auto cat : ComponentAdaptorTypeManager::all())
    {
        if (cat != nullptr &&
            cat->architecture() == _architecture &&
            cat->componentType() == component->type() &&
            component->type()->isCompatibleWith(_type))
        {   //  Yes!
            _adaptedComponents.insert(component);
            component->_owner = this;
            auto adaptor = cat->createAdaptor(this, component);
            _adaptors.insert(adaptor);
            return;
        }
    }
    //  OOPS! Give up
    throw IncompatibleComponentException(component);
}

void VirtualMachine::removeComponent(IComponent * component)
{
    Q_ASSERT(component != nullptr);
    //  Synchronize IN ORDER, so there can't be a deadlock
    emuone::util::Lock _1(_guard);
    emuone::util::Lock _2(component->guard);

    Q_ASSERT(component->owner() == this);   //  TODO throw instead
    //  Native ?
    if (_nativeComponents.contains(component))
    {
        _nativeComponents.remove(component);
        component->_owner = nullptr;
        return;
    }
    //  Adapted ?
    if (_adaptedComponents.contains(component))
    {
        auto adaptor = findAdaptor(component);
        Q_ASSERT(adaptor != nullptr);
        _adaptedComponents.remove(component);
        component->_owner = nullptr;
        _adaptors.remove(adaptor);
        delete adaptor;
        return;
    }
    //  OOPS! Should never happen!
    Q_ASSERT(false);
}

QString VirtualMachine::toAbsolutePath(const QString & relativePath)
{
    if (QFileInfo(relativePath).isAbsolute())
    {   //  Already absolute
        return QFileInfo(relativePath).canonicalFilePath();
    }
    //  Try ansolutizing
    QDir baseDir(QFileInfo(_location).absolutePath());
    QString result = baseDir.absoluteFilePath(relativePath);
    return QFileInfo(result).canonicalFilePath();
}

QString VirtualMachine::toRelatimePath(const QString & absolutePath)
{
    QDir baseDir(QFileInfo(_location).absolutePath());
    QString result = baseDir.relativeFilePath(absolutePath);
    if (result.contains(".."))
    {   //  Can't go above the VM directory
        return absolutePath;
    }
    else if (result != ".")
    {   //  Prepend sub-dirs with ./ prefx
        result = "./" + result;
    }
    return result;
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


void VirtualMachine::start()
{
    {   //  State changes must occur in "locked"ode
        emuone::util::Lock _(_guard);

        if (_state != State::Stopped)
        {   //  OOPS!
            throw InvalidVirtualMachineStateException();
        }
        try
        {
            _connectComponents();   //  may throw
            _initializeComponents();//  may throw
            _startCompoonents();    //  may throw
            //  Start sequence successful
            _state = State::Running;
        }
        catch (...)
        {   //  OOPS! Cleanup & re-throw
            _stopCompoonents();
            _deinitializeComponents();
            _disconnectComponents();
        }
    }
    //  TODO emit state change signal in "unlocked" mode
}

void VirtualMachine::stop() noexcept
{
    {   //  State changes must occur in "locked"ode
        emuone::util::Lock _(_guard);

        if (_state == State::Running)
        {   //  Must stop & cleanup
            _stopCompoonents();
            _deinitializeComponents();
            _disconnectComponents();
            _state = State::Stopped;
        }
        else if (_state == State::Suspended)
        {   //  Must destroy runtime state
            Q_ASSERT(false);    //  TODO implement
            _state = State::Stopped;
        }
    }
    //  TODO emit state change signal in "unlocked" mode
}

void VirtualMachine::suspend()
{
    emuone::util::Lock _(_guard);
    //  TODO implement
    Q_ASSERT(false);
}

void VirtualMachine::resume()
{
    emuone::util::Lock _(_guard);
    //  TODO implement
    Q_ASSERT(false);
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
    for (auto c : components())
    {
        QDomElement componentElement = document.createElement("Component");
        rootElement.appendChild(componentElement);
        c->saveConfiguration(componentElement);
    }

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

    //  Components
    for (auto componentElement = rootElement.firstChildElement("Component");
         !componentElement.isNull();
         componentElement = componentElement.nextSiblingElement("Component"))
    {
        auto typeMnemonic = componentElement.attribute("Type");
        auto componentType = ComponentTypeManager::find(typeMnemonic);
        if (componentType == nullptr)
        {   //  OOPS! TODO throw
            Q_ASSERT(false);
        }
        auto component = componentType->createComponent();
        component->restoreConfiguration(componentElement);
        //  Is there an adaptor involved ?
        IComponentAdaptorType * adaptorType = nullptr;
        auto adaptorElement = componentElement.firstChildElement("Adaptor");
        if (!adaptorElement.isNull())
        {   //  Yes
            adaptorType =
                ComponentAdaptorTypeManager::find(
                    adaptorElement.attribute("Type"));
        }
        //  Add component to VM
        try
        {
            vm->addComponent(component, adaptorType);   //  may throw
        }
        catch (...)
        {   //  OOPS! Cleanup & re-throw
            delete component;
            throw;
        }
        //  If there IS an adaptor, load its configuration
        if (auto adaptor = vm->findAdaptor(component);
            adaptor != nullptr && !adaptorElement.isNull())
        {   //  Guard against adapted component definition missing Adaptor sub-element
            adaptor->restoreConfiguration(adaptorElement);
        }
    }

    //  All done
    return vm.release();
}

//////////
//  Implementation helpers
void VirtualMachine::_connectComponents()
{
    for (auto c : _nativeComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Constructed);
        c->connect();   //  may throw
        Q_ASSERT(c->state() == IComponent::State::Connected);
    }
    for (auto c : _adaptedComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Constructed);
        c->connect();   //  may throw
        Q_ASSERT(c->state() == IComponent::State::Connected);
    }
    for (auto a : _adaptors)
    {
        Q_ASSERT(a->state() == IComponentAdaptor::State::Constructed);
        a->connect();   //  may throw
        Q_ASSERT(a->state() == IComponentAdaptor::State::Connected);
    }
}

void VirtualMachine::_initializeComponents()
{
    for (auto c : _nativeComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Connected);
        c->initialize();    //  may throw
        Q_ASSERT(c->state() == IComponent::State::Initialized);
    }
    for (auto c : _adaptedComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Connected);
        c->initialize();    //  may throw
        Q_ASSERT(c->state() == IComponent::State::Initialized);
    }
    for (auto a : _adaptors)
    {
        Q_ASSERT(a->state() == IComponentAdaptor::State::Connected);
        a->initialize();    //  may throw
        Q_ASSERT(a->state() == IComponentAdaptor::State::Initialized);
    }
}

void VirtualMachine::_startCompoonents()
{
    for (auto c : _nativeComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Initialized);
        c->start(); //  may throw
        Q_ASSERT(c->state() == IComponent::State::Running);
    }
    for (auto c : _adaptedComponents)
    {
        Q_ASSERT(c->state() == IComponent::State::Initialized);
        c->start(); //  may throw
        Q_ASSERT(c->state() == IComponent::State::Running);
    }
    for (auto a : _adaptors)
    {
        Q_ASSERT(a->state() == IComponentAdaptor::State::Initialized);
        a->start(); //  may throw
        Q_ASSERT(a->state() == IComponentAdaptor::State::Running);
    }
}

void VirtualMachine::_stopCompoonents()
{   //  Any Running component must be Stopped
    for (auto a : _adaptors)
    {
        if (a->state() == IComponentAdaptor::State::Running)
        {   //  Guard needed when recovering from failed partial start
            a->stop();  //  npexcept
            Q_ASSERT(a->state() == IComponentAdaptor::State::Initialized);
        }
    }
    for (auto c : _adaptedComponents)
    {   //  Guard needed when recovering from failed partial start
        if (c->state() == IComponent::State::Running)
        {
            c->stop();  //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Initialized);
        }
    }
    for (auto c : _nativeComponents)
    {   //  Guard needed when recovering from failed partial start
        if (c->state() == IComponent::State::Running)
        {
            c->stop();  //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Initialized);
        }
    }
}

void VirtualMachine::_deinitializeComponents()
{   //  Any Initialized component must be Deinitialized
    for (auto a : _adaptors)
    {
        if (a->state() == IComponentAdaptor::State::Initialized)
        {   //  Guard needed when recovering from failed partial start
            a->deinitialize();  //  npexcept
            Q_ASSERT(a->state() == IComponentAdaptor::State::Connected);
        }
    }
    for (auto c : _adaptedComponents)
    {
        if (c->state() == IComponent::State::Initialized)
        {   //  Guard needed when recovering from failed partial start
            c->deinitialize();  //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Connected);
        }
    }
    for (auto c : _nativeComponents)
    {
        if (c->state() == IComponent::State::Initialized)
        {   //  Guard needed when recovering from failed partial start
            c->deinitialize();  //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Connected);
        }
    }
}

void VirtualMachine::_disconnectComponents()
{   //  Any Connected component must be Disconnected
    for (auto a : _adaptors)
    {
        if (a->state() == IComponentAdaptor::State::Connected)
        {   //  Guard needed when recovering from failed partial start
            a->disconnect();    //  npexcept
            Q_ASSERT(a->state() == IComponentAdaptor::State::Constructed);
        }
    }
    for (auto c : _adaptedComponents)
    {
        if (c->state() == IComponent::State::Connected)
        {   //  Guard needed when recovering from failed partial start
            c->disconnect();    //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Constructed);
        }
    }
    for (auto c : _nativeComponents)
    {
        if (c->state() == IComponent::State::Connected)
        {   //  Guard needed when recovering from failed partial start
            c->disconnect();    //  noexcept
            Q_ASSERT(c->state() == IComponent::State::Constructed);
        }
    }
}


//  End of emuone-core/VirtualMachine.cpp
