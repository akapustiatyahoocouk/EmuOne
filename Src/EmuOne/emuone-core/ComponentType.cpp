//
//  emuone-core/ComponentType.cpp
//
//  The ComponentType class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

ComponentTypeList ComponentType::_registry;

//////////
//  Operations
bool ComponentType::isAdaptableTo(const Architecture * architecture) const
{
    Q_ASSERT(architecture != nullptr);

    if (this->isCompatibleWith(architecture))
    {   //  OOPS! No need to "adapt"!
        return false;
    }
    for (AdaptorType * adaptorType : AdaptorType::all())
    {
        if (adaptorType->isCompatibleWith(architecture) && adaptorType->isCompatibleWith(this))
        {
            return true;
        }
    }
    return false;
}

//////////
//  Registry
ComponentTypeList ComponentType::all()
{
    return _registry;
}

ComponentType * ComponentType::findByMnemonic(const QString & mnemonic)
{
    for (ComponentType * componentType : _registry)
    {
        if (componentType->mnemonic() == mnemonic)
        {
            return componentType;
        }
    }
    return nullptr;
}

bool ComponentType::registerComponentType(ComponentType * componentType)
{
    Q_ASSERT(componentType != nullptr);
    Q_ASSERT(componentType->category() != nullptr);

    if (!ComponentCategory::registerComponentCategory(componentType->category()))
    {   //  OOPS! No point in continuing
        return false;
    }
    ComponentType * registeredComponentType = findByMnemonic(componentType->mnemonic());
    if (registeredComponentType == nullptr)
    {
        _registry.append(componentType);
        return true;
    }
    return registeredComponentType == componentType;
}

//  End of emuone-core/ComponentType.cpp
