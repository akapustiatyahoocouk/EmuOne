//
//  emuone-core/Architecture.cpp
//
//  The Architecture class implementation
//
//////////
#include "emuone-core/API.hpp"

ArchitectureList Architecture::_registry;

//////////
//  Registry
ArchitectureList Architecture::getAll()
{
    return _registry;
}

Architecture * Architecture::findByMnemonic(const QString & mnemonic)
{
    for (Architecture * architecture : _registry)
    {
        if (architecture->getMnemonic() == mnemonic)
        {
            return architecture;
        }
    }
    return nullptr;
}

bool Architecture::registerArchitecture(Architecture * architecture)
{
    Q_ASSERT(architecture != nullptr);

    Architecture * registeredArchitecture = findByMnemonic(architecture->getMnemonic());
    if (registeredArchitecture == nullptr)
    {
        _registry.append(registeredArchitecture);
        return true;
    }
    return registeredArchitecture == architecture;
}

//  End of emuone-core/Architecture.cpp
