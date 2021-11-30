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
    rootElement.setAttribute("Type", this->type()->mnemonic());
    rootElement.setAttribute("Name", _name);
    rootElement.setAttribute("Architecture", _architecture->mnemonic());
    rootElement.setAttribute("Template", _template->mnemonic());

    //  Create & populate component nodes
    for (Component * component : _components)
    {
        QDomElement componentElement = domDocument.createElement("Component");
        rootElement.appendChild(componentElement);
        componentElement.setAttribute("Type", component->type()->mnemonic());
        componentElement.setAttribute("Name", component->name());
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
    if (component->type()->isCompatibleWith(_architecture))
    {   //  Yes!
        _components.append(component);
        _compatibleComponents.append(component);
        component->_virtualAppliance = this;
        return;
    }

    //  Is the "component" adaptable to this architecture ?
    //  TODO

    //  Give uo
    throw VirtualApplianceException(component->type()->displayName() + " is not compatible with " + _architecture->displayName());
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

ComponentList VirtualAppliance::components() const
{
    return _components;
}

ComponentList VirtualAppliance::components(ComponentCategory * componentCategory)
{
    Q_ASSERT(componentCategory != nullptr);

    ComponentList result;
    for (auto component : _components)
    {
        if (component->type()->category() == componentCategory)
        {
            result.append(component);
        }
    }
    return result;
}

AdaptorList VirtualAppliance::adaptors(ComponentCategory * componentCategory)
{
    Q_ASSERT(componentCategory != nullptr);

    AdaptorList result;
    for (auto adaptor : _adaptors)
    {
        if (adaptor->type()->category() == componentCategory)
        {
            result.append(adaptor);
        }
    }
    return result;
}

QString VirtualAppliance::toRelativePath(const QString & path) const
{
    QDir vaDir(QFileInfo(_location).absoluteDir());
    QString relativePath = vaDir.relativeFilePath(path);
    if (!relativePath.contains(".."))
    {   //  Good to go, but we want it to look like a path still
        if (!relativePath.contains("/"))
        {   //  ...so prepend with "./"
            relativePath = "./" + relativePath;
        }
        return relativePath;
    }
    return path;
}

QString VirtualAppliance::toAbsolutePath(const QString & path) const
{
    if (QFileInfo(path).isRelative())
    {   //  Convert to absolute using VA's location as starting point
        return QFileInfo(QFileInfo(_location).dir(), path).canonicalFilePath();
    }
    return path;
}

//////////
//  Operations (state control)
VirtualAppliance::State VirtualAppliance::state() const
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
        throw;
    }

    //  If one or more Components of this VA implement the IClockedComponentAspect, they
    //  need the VA-wide "master clock" thread to drive them
    QList<IClockedComponentAspect*> clockedComponents = this->findComponentsByRole<IClockedComponentAspect>();
    if (!clockedComponents.isEmpty())
    {   //  Yes! A "master clock" thread is needed!
        QMap<IClockedComponentAspect*, int> clockSchedule = _calculateClockSchedule(clockedComponents);
        _masterClockThread = new _MasterClockThread(this, clockSchedule);
        _masterClockThread->start();
    }

    //  Done
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

    if (_masterClockThread != nullptr)
    {
        _masterClockThread->requestStop();
        _masterClockThread->wait();
        _masterClockThread = nullptr;
    }
    _state = State::Stopped;

    //  TODO is the VA was "suspended", must also delete the saved "runtime state"
}

void VirtualAppliance::suspend()
{
    QMutexLocker locker(&_stateGuard);
    if (this != reinterpret_cast<VirtualAppliance*>(uintptr_t(1)))
    {   //  TODO implement properly
        throw VirtualApplianceException("Not yet implemented");
    }
}

void VirtualAppliance::resume()
{
    QMutexLocker locker(&_stateGuard);
    if (this != reinterpret_cast<VirtualAppliance*>(uintptr_t(1)))
    {   //  TODO implement properly
        throw VirtualApplianceException("Not yet implemented");
    }
}

//////////
//  Implementation helpers
void VirtualAppliance::_connectComponents()
{
    try
    {
        for (Component * component : _components)
        {
            Q_ASSERT(component->state() == Component::State::Constructed);
            component->connect();
            Q_ASSERT(component->state() == Component::State::Connected);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->state() == Component::State::Constructed);
            adaptor->connect();
            Q_ASSERT(adaptor->state() == Component::State::Connected);
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
            Q_ASSERT(component->state() == Component::State::Connected);
            component->initialise();
            Q_ASSERT(component->state() == Component::State::Initialised);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->state() == Component::State::Connected);
            adaptor->initialise();
            Q_ASSERT(adaptor->state() == Component::State::Initialised);
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
            Q_ASSERT(component->state() == Component::State::Initialised);
            component->start();
            Q_ASSERT(component->state() == Component::State::Running);
        }
        for (Adaptor * adaptor : _adaptors)
        {
            Q_ASSERT(adaptor->state() == Component::State::Initialised);
            adaptor->start();
            Q_ASSERT(adaptor->state() == Component::State::Running);
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
        if (adaptor->state() == Component::State::Running)
        {
            adaptor->stop();
            Q_ASSERT(adaptor->state() == Component::State::Initialised);
        }
    }
    for (Component * component : _components)
    {
        if (component->state() == Component::State::Running)
        {
            component->stop();
            Q_ASSERT(component->state() == Component::State::Initialised);
        }
    }
}

void VirtualAppliance::_deinitialiseComponents()
{
    for (Adaptor * adaptor : _adaptors)
    {
        if (adaptor->state() == Component::State::Initialised)
        {
            adaptor->deinitialise();
            Q_ASSERT(adaptor->state() == Component::State::Connected);
        }
    }
    for (Component * component : _components)
    {
        if (component->state() == Component::State::Initialised)
        {
            component->deinitialise();
            Q_ASSERT(component->state() == Component::State::Connected);
        }
    }
}

void VirtualAppliance::_disconnectComponents()
{
    for (Adaptor * adaptor : _adaptors)
    {
        if (adaptor->state() == Component::State::Connected)
        {
            adaptor->disconnect();
            Q_ASSERT(adaptor->state() == Component::State::Constructed);
        }
    }
    for (Component * component : _components)
    {
        if (component->state() == Component::State::Connected)
        {
            component->disconnect();
            Q_ASSERT(component->state() == Component::State::Constructed);
        }
    }
}

QMap<IClockedComponentAspect*, int> VirtualAppliance::_calculateClockSchedule(const QList<IClockedComponentAspect*> & clockedComponents)
{
    Q_ASSERT(!clockedComponents.isEmpty());

    //  What's the fastest clocked component ?
    IClockedComponentAspect * fastestClockedComponent = nullptr;
    for (IClockedComponentAspect * clockedComponent : clockedComponents)
    {
        if (fastestClockedComponent == nullptr ||
            clockedComponent->clockFrequency() > fastestClockedComponent->clockFrequency())
        {
            fastestClockedComponent = clockedComponent;
        }
    }
    Q_ASSERT(fastestClockedComponent->clockFrequency().toHz() > 0);

    //  What's the slowest clocked component ?
    IClockedComponentAspect * slowestClockedComponent = nullptr;
    for (IClockedComponentAspect * clockedComponent : clockedComponents)
    {
        if (slowestClockedComponent == nullptr ||
            clockedComponent->clockFrequency() < slowestClockedComponent->clockFrequency())
        {
            slowestClockedComponent = clockedComponent;
        }
    }
    Q_ASSERT(slowestClockedComponent->clockFrequency().toHz() > 0);

    //  Calculate the fastest/slowest "clock frequency" for all components.
    //  We do not want a difference of more than 1000, or else the "clock
    //  tick schedule" will become too long
    ClockFrequency fastestClockFrequency = fastestClockedComponent->clockFrequency();
    ClockFrequency slowestClockFrequency = slowestClockedComponent->clockFrequency();
    if (slowestClockFrequency < fastestClockFrequency / 1000)
    {
        slowestClockFrequency = fastestClockFrequency / 1000;
        if (slowestClockFrequency.toHz() == 0)
        {   //  OOPS! Underflow!
            slowestClockFrequency = ClockFrequency(ClockFrequency::Unit::HZ, 1);    //  that's the smallest we can express
        }
    }

    //  Prepare the "clock tick" schedule
    QMap<IClockedComponentAspect*, int> result;
    for (IClockedComponentAspect * clockedComponent : clockedComponents)
    {
        int numTicks = clockedComponent->clockFrequency() / slowestClockFrequency;
        result.insert(clockedComponent, numTicks);
    }

    //  Done
    return result;
}

//////////
//  VirtualAppliance::_MasterClockThread
VirtualAppliance::_MasterClockThread::_MasterClockThread(VirtualAppliance * virtualAppliance, const QMap<IClockedComponentAspect*, int> & clockSchedule)
    :   _virtualAppliance(virtualAppliance)
{
    //  How many "clock ticks" per one complete cycle ?
    _numClockTickReceivers = 0;
    for (IClockedComponentAspect * clockedComponent : clockSchedule.keys())
    {
        int oldNumClockTickReceivers = _numClockTickReceivers;
        _numClockTickReceivers = clockSchedule[clockedComponent];
        Q_ASSERT(_numClockTickReceivers > oldNumClockTickReceivers);    //  No overflows!!!
    }
    _clockTickReceivers = new IClockedComponentAspect*[static_cast<size_t>(_numClockTickReceivers)];

    //  Fill the cycle...
    _numClockTickReceivers = 0;
    for (IClockedComponentAspect * clockedComponent : clockSchedule.keys())
    {
        for (int i = 0; i < clockSchedule[clockedComponent]; i++)
        {
            _clockTickReceivers[_numClockTickReceivers++] = clockedComponent;
        }
    }

    //  ...and randomise the order in which "clock ticks" are processed
    for (int i = 0; i < _numClockTickReceivers; i++)
    {
        int j = rand() % _numClockTickReceivers;
        std::swap(_clockTickReceivers[i], _clockTickReceivers[j]);
    }
}

VirtualAppliance::_MasterClockThread::~_MasterClockThread()
{
    delete [] _clockTickReceivers;
}

void VirtualAppliance::_MasterClockThread::run()
{
    //  Which clock frequency we're supposed to be running at ?
    uint64_t clockFrequencyHz = 0;
    for (int i = 0; i < _numClockTickReceivers; i++)
    {
        clockFrequencyHz = std::max(clockFrequencyHz, _clockTickReceivers[i]->clockFrequency().toHz());
    }

    //  Go!
    while (!_stopRequested)
    {
    }
}

//  End of emuone-core/VirtualAppliance.cpp
