//
//  emuone-core/ComponentCategory.cpp
//
//  The core::ComponentCategory class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

ComponentCategoryList ComponentCategory::_registry;

//////////
//  Operations
ComponentTypeList ComponentCategory::componentTypes() const
{
    ComponentTypeList result;
    for (ComponentType * componentType : ComponentType::all())
    {
        if (componentType->category() == this)
        {
            result.append(componentType);
        }
    }
    return result;
}

//////////
//  Registry
ComponentCategoryList ComponentCategory::all()
{
    return _registry;
}

ComponentCategory * ComponentCategory::findByMnemonic(const QString & mnemonic)
{
    for (ComponentCategory * componentCategory : _registry)
    {
        if (componentCategory->mnemonic() == mnemonic)
        {
            return componentCategory;
        }
    }
    return nullptr;
}

bool ComponentCategory::registerComponentCategory(ComponentCategory * componentCategory)
{
    Q_ASSERT(componentCategory != nullptr);

    ComponentCategory * registeredComponentCategory = findByMnemonic(componentCategory->mnemonic());
    if (registeredComponentCategory == nullptr)
    {
        _registry.append(componentCategory);
        return true;
    }
    return registeredComponentCategory == componentCategory;
}

//  End of emuone-core/ComponentCategory.cpp
