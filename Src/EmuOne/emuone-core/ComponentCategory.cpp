//
//  emuone-core/ComponentCategory.cpp
//
//  The ComponentCategory class implementation
//
//////////
#include "emuone-core/API.hpp"

ComponentCategoryList ComponentCategory::_registry;

//////////
//  Operations

//////////
//  Registry
ComponentCategoryList ComponentCategory::getAll()
{
    return _registry;
}

ComponentCategory * ComponentCategory::findByMnemonic(const QString & mnemonic)
{
    for (ComponentCategory * componentCategory : _registry)
    {
        if (componentCategory->getMnemonic() == mnemonic)
        {
            return componentCategory;
        }
    }
    return nullptr;
}

bool ComponentCategory::registerComponentCategory(ComponentCategory * componentCategory)
{
    Q_ASSERT(componentCategory != nullptr);

    ComponentCategory * registeredComponentCategory = findByMnemonic(componentCategory->getMnemonic());
    if (registeredComponentCategory == nullptr)
    {
        _registry.append(componentCategory);
        return true;
    }
    return registeredComponentCategory == componentCategory;
}

//  End of emuone-core/ComponentCategory.cpp
