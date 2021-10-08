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
