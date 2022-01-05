//
//  emuone-core/AdaptorType.cpp
//
//  The core::AdaptorType class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  util::StockObject
QIcon AdaptorType::smallIcon() const
{
    static QIcon icon;

    if (icon.isNull())
    {
        icon = QIcon(":/emuone/core/Images/AdaptorSmall.png");
    }
    return icon;
}

QIcon AdaptorType::largeIcon() const
{
    static QIcon icon;

    if (icon.isNull())
    {
        icon = QIcon(":/emuone/core/Images/AdaptorLarge.png");
    }
    return icon;
}

//////////
//  Registry
AdaptorTypeList AdaptorType::_registry;

AdaptorTypeList AdaptorType::all()
{
    return _registry;
}

AdaptorType * AdaptorType::findByMnemonic(const QString & mnemonic)
{
    for (AdaptorType * adaptorType : _registry)
    {
        if (adaptorType->mnemonic() == mnemonic)
        {
            return adaptorType;
        }
    }
    return nullptr;
}

bool AdaptorType::registerAdaptorType(AdaptorType * adaptorType)
{
    Q_ASSERT(adaptorType != nullptr);

    AdaptorType * registeredAdaptorType = findByMnemonic(adaptorType->mnemonic());
    if (registeredAdaptorType == nullptr)
    {
        _registry.append(adaptorType);
        return true;
    }
    return registeredAdaptorType == adaptorType;
}

AdaptorType * AdaptorType::find(Architecture * architecture, ComponentType * componentType)
{
    Q_ASSERT(architecture != nullptr);
    Q_ASSERT(componentType != nullptr);

    for (AdaptorType * adaptorType : _registry)
    {
        if (adaptorType->isCompatibleWith(architecture) && adaptorType->isCompatibleWith(componentType))
        {
            return adaptorType;
        }
    }
    return nullptr;
}

//  End of emuone-core/AdaptorType.cpp
