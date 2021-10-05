//
//  emuone-core/VirtualAppliance.cpp
//
//  The VirtualAppliance class implementation
//
//////////
#include "emuone-core/API.hpp"

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
        return;
    }

    //  Is the "component" adaptable to this architecture ?
    //  TODO

    //  Give up
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
}

void VirtualAppliance::stop()
{
}

void VirtualAppliance::suspend()
{
    throw VirtualApplianceException("Not yet implemented");
}

void VirtualAppliance::resume()
{
    throw VirtualApplianceException("Not yet implemented");
}

//  End of emuone-core/VirtualAppliance.cpp
