//
//  emuone-core/VirtualAppliance.cpp
//
//  The VirtualAppliance class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Constants
const QString VirtualAppliance::PreferredExtension = "emuone";

//////////
//  Construction/destruction
VirtualAppliance::VirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate)
    :   _name(name),
        _location(location),
        _architecture(architecture),
        _template(virtualApplianceTemplate)
{
    Q_ASSERT(_architecture != nullptr);
    Q_ASSERT(_template != nullptr);
}

VirtualAppliance::~VirtualAppliance()
{
}

//////////
//  Operations
bool VirtualAppliance::isValidName(const QString & name)
{
    return name.length() > 0 && name.length() <= 255 &&
           name.trimmed().length() == name.length();
}

void VirtualAppliance::setName(const QString & name)
{
    Q_ASSERT(isValidName(name));
    _name = name;
}

void VirtualAppliance::save() const
{
    QDomDocument domDocument;
    //  Create & populate roor node
    QDomElement rootElement = domDocument.createElement("VirtualAppliance");
    domDocument.appendChild(rootElement);
    rootElement.setAttribute("Type", this->getType()->getMnemonic());
    rootElement.setAttribute("Name", _name);
    rootElement.setAttribute("Architecture", _architecture->getMnemonic());
    rootElement.setAttribute("Template", _template->getMnemonic());

    //  Create & populate component nodes
    for (Component * component : _components)
    {
        QDomElement componentElement = domDocument.createElement("Component");
        rootElement.appendChild(componentElement);
        componentElement.setAttribute("Type", component->getType()->getMnemonic());
        componentElement.setAttribute("Name", component->getName());
        //  ...and its configuration too
        QDomElement configurationElement = domDocument.createElement("Configuration");
        componentElement.appendChild(configurationElement);
        component->serialiseConfiguration(configurationElement);
    }

    //  Save
    QFile file(_location);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        //  TODO use file.error() to improve diagnostic message
        throw VirtualApplianceException("Failed to save VM confiuguration to " + _location);
    }
    QTextStream stream(&file);
    stream << domDocument.toString();
    file.close();
}

VirtualAppliance * VirtualAppliance::load(const QString & location)
{
    //  Load XML document & analyse root element
    QDomDocument domDocument;
    QFile file(location);
    if (!file.open(QIODevice::ReadOnly))
    {   //  OOPS!
        //  TODO use file.error() to improve diagnostic message
        throw VirtualApplianceException("Failed to load VM confiuguration from " + location);
    }
    if (!domDocument.setContent(&file))
    {   //  OOPS!
        file.close();
        //  TODO use file.error() to improve diagnostic message
        throw VirtualApplianceException("Failed to load VM confiuguration from " + location);
    }
    file.close();

    QDomElement rootElement = domDocument.documentElement();
    if (rootElement.isNull() || rootElement.tagName() != "VirtualAppliance")
    {
        throw VirtualApplianceException(location + " is not a valid VM confiuguration file");
    }
    QString typeMnemonic = rootElement.attribute("Type");
    QString name = rootElement.attribute("Name");
    QString architectureMnemonic = rootElement.attribute("Architecture");
    QString templateMnemonic = rootElement.attribute("Template");

    VirtualApplianceType * virtualApplianceType = VirtualApplianceType::findByMnemonic(typeMnemonic);
    if (virtualApplianceType == nullptr)
    {
        throw VirtualApplianceException("Unsupported virtual appliance type " + typeMnemonic);
    }
    if (!VirtualAppliance::isValidName(name))
    {
        throw VirtualApplianceException("Invalid virtual appliance name");
    }
    Architecture * architecture = Architecture::findByMnemonic(architectureMnemonic);
    if (architecture == nullptr)
    {
        throw VirtualApplianceException("Unsupported virtual appliance architecture " + architectureMnemonic);
    }
    VirtualApplianceTemplate * virtualApplianceTemplate = architecture->findVirtualApplianceTemplate(templateMnemonic);
    if (virtualApplianceTemplate == nullptr)
    {
        throw VirtualApplianceException("Unsupported virtual appliance template " + templateMnemonic);
    }

    //  Create a new VA
    VirtualAppliance * virtualAppliance =
        virtualApplianceType->createVirtualAppliance(name,
                                                     location,
                                                     architecture,
                                                     virtualApplianceTemplate);

    //  Create components
    for (QDomElement componentElement = rootElement.firstChildElement();
         !componentElement.isNull();
         componentElement = componentElement.nextSiblingElement())
    {
        if (componentElement.tagName() != "Component")
        {   //  Ignore this one
            continue;
        }
        //  Instantiate the Component...
        QString componentTypeMnemonic = componentElement.attribute("Type");
        QString componentName = componentElement.attribute("Name");
        ComponentType * componentType = ComponentType::findByMnemonic(componentTypeMnemonic);
        if (componentType == nullptr)
        {   //  OOPS!
            throw VirtualApplianceException("Unsupported component type " + componentTypeMnemonic);
        }
        if (!Component::isValidName(componentName))
        {   //  OOPS!
            throw VirtualApplianceException("Invalid component name " + componentName);
        }
        Component * component = componentType->createComponent();
        component->setName(componentName);
        //  ...and load its configuration
        for (QDomElement configurationElement = componentElement.firstChildElement();
             !configurationElement.isNull();
             configurationElement = configurationElement.nextSiblingElement())
        {
            if (configurationElement.tagName() == "Configuration")
            {
                component->deserialiseConfiguration(configurationElement);
            }
        }
        virtualAppliance->addComponent(component);
    }

    //  Done
    return virtualAppliance;
}

void VirtualAppliance::addComponent(Component * component)
{
    Q_ASSERT(component != nullptr);

    QMutexLocker locker(&_stateGuard);
    if (_state != State::Stopped)
    {
        throw VirtualApplianceException("Virtual appliance must be Stopped");
    }

    if (_components.contains(component))
    {   //  No harm done
        return;
    }
    if (component->_virtualAppliance != nullptr)
    {   //  OOPS!
        throw VirtualApplianceException("Component already in use");
    }

    //  Is the "component" compatble with this VA's architecture ?
    if (component->getType()->isCompatibleWith(_architecture))
    {   //  Yes!
        _components.append(component);
        _compatibleComponents.append(component);
        component->_virtualAppliance = this;
        return;
    }

    //  Is the "component" adaptable to this architecture ?
    //  TODO

    //  Give uo
    throw VirtualApplianceException(component->getType()->getDisplayName() + " is not compatible with " + _architecture->getDisplayName());
}

void VirtualAppliance::removeComponent(Component * component)
{
    Q_ASSERT(component != nullptr);

    QMutexLocker locker(&_stateGuard);
    if (_state != State::Stopped)
    {
        throw VirtualApplianceException("Virtual appliance must be Stopped");
    }

    if (!_components.contains(component) || component->_virtualAppliance != this)
    {   //  No harm done
        return;
    }

    //  Is the "component" compatble with this VA's architecture ?
    if (_compatibleComponents.contains(component))
    {
        _components.removeOne(component);
        _compatibleComponents.removeOne(component);
        component->_virtualAppliance = nullptr;
        return;
    }

    //  Is the "component" adaptable to this architecture ?
    //  TODO

    //  Give up
}

ComponentList VirtualAppliance::getComponents() const
{
    return _components;
}

ComponentList VirtualAppliance::getComponents(ComponentCategory * componentCategory)
{
    Q_ASSERT(componentCategory != nullptr);

    ComponentList result;
    for (auto component : _components)
    {
        if (component->getType()->getCategory() == componentCategory)
        {
            result.append(component);
        }
    }
    return result;
}

//////////
//  Operations (state control)
VirtualAppliance::State VirtualAppliance::getState() const
{
    QMutexLocker locker(&_stateGuard);
    return _state;
}

void VirtualAppliance::start()
{
    QMutexLocker locker(&_stateGuard);

    if (_state != State::Stopped)
    {   //  OOPS! Can't!
        return;
    }

    try
    {
        _connectComponents();
        _initialiseComponents();
        _startComponents();
    }
    catch (...)
    {   //  OOPS! Cleanup & re-throw
        _stopComponents();
        _deinitialiseComponents();
        _disconnectComponents();
    }

    _state = State::Running;
}

void VirtualAppliance::stop() noexcept
{
    QMutexLocker locker(&_stateGuard);

    if (_state == State::Stopped)
    {   //  OOPS! Can't!
        return;
    }

    _stopComponents();
    _deinitialiseComponents();
    _disconnectComponents();

    _state = State::Stopped;

    //  TODO is the VA was "suspended", must also delete the saved "runtime state"
}

void VirtualAppliance::suspend()
{
    QMutexLocker locker(&_stateGuard);
    throw VirtualApplianceException("Not yet implemented");
}

void VirtualAppliance::resume()
{
    QMutexLocker locker(&_stateGuard);
    throw VirtualApplianceException("Not yet implemented");
}

//////////
//  Implementation helpers
void VirtualAppliance::_connectComponents()
{
    try
    {
        for (Component * component : _components)
        {
            Q_ASSERT(component->getState() == Component::State::Constructed);
            component->connect();
            Q_ASSERT(component->getState() == Component::State::Connected);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->getState() == Component::State::Constructed);
            adaptor->connect();
            Q_ASSERT(adaptor->getState() == Component::State::Connected);
        }
    }
    catch (...)
    {   //  OOPS! Rollback and re-throw
        _disconnectComponents();
        throw;
    }
}

void VirtualAppliance::_initialiseComponents()
{
    try
    {
        for (Component * component : _components)
        {
            Q_ASSERT(component->getState() == Component::State::Connected);
            component->initialise();
            Q_ASSERT(component->getState() == Component::State::Initialised);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->getState() == Component::State::Connected);
            adaptor->initialise();
            Q_ASSERT(adaptor->getState() == Component::State::Initialised);
        }
    }
    catch (...)
    {   //  OOPS! Rollback and re-throw
        _deinitialiseComponents();
        throw;
    }
}

void VirtualAppliance::_startComponents()
{
    try
    {
        for (Component * component : _components)
        {
            Q_ASSERT(component->getState() == Component::State::Initialised);
            component->start();
            Q_ASSERT(component->getState() == Component::State::Running);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->getState() == Component::State::Initialised);
            adaptor->start();
            Q_ASSERT(adaptor->getState() == Component::State::Running);
        }
    }
    catch (...)
    {   //  OOPS! Rollback and re-throw
        _stopComponents();
        throw;
    }
}

void VirtualAppliance::_stopComponents()
{
    for (Adaptor * adaptor : _adaptors)
    {
        if (adaptor->getState() == Component::State::Running)
        {
            adaptor->stop();
            Q_ASSERT(adaptor->getState() == Component::State::Initialised);
        }
    }
    for (Component * component : _components)
    {
        if (component->getState() == Component::State::Running)
        {
            component->stop();
            Q_ASSERT(component->getState() == Component::State::Initialised);
        }
    }
}

void VirtualAppliance::_deinitialiseComponents()
{
    for (Adaptor * adaptor : _adaptors)
    {
        if (adaptor->getState() == Component::State::Initialised)
        {
            adaptor->deinitialise();
            Q_ASSERT(adaptor->getState() == Component::State::Connected);
        }
    }
    for (Component * component : _components)
    {
        if (component->getState() == Component::State::Initialised)
        {
            component->deinitialise();
            Q_ASSERT(component->getState() == Component::State::Connected);
        }
    }
}

void VirtualAppliance::_disconnectComponents()
{
    for (Adaptor * adaptor : _adaptors)
    {
        if (adaptor->getState() == Component::State::Connected)
        {
            adaptor->disconnect();
            Q_ASSERT(adaptor->getState() == Component::State::Constructed);
        }
    }
    for (Component * component : _components)
    {
        if (component->getState() == Component::State::Connected)
        {
            component->disconnect();
            Q_ASSERT(component->getState() == Component::State::Constructed);
        }
    }
}

//  End of emuone-core/VirtualAppliance.cpp
