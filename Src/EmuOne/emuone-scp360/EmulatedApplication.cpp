//
//  emuone-scp360/EmulatedApplication.cpp
//
//  The scp360::EmulatedApplication class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

EmulatedApplicationList EmulatedApplication::_registry;

//////////
//  Registry
EmulatedApplicationList EmulatedApplication::all()
{
    return _registry;
}

EmulatedApplication * EmulatedApplication::findByMnemonic(const QString & mnemonic)
{
    for (EmulatedApplication * emulatedApplication : _registry)
    {
        if (emulatedApplication->mnemonic() == mnemonic)
        {
            return emulatedApplication;
        }
    }
    return nullptr;
}

bool EmulatedApplication::registerEmulatedApplication(EmulatedApplication * emulatedApplication)
{
    Q_ASSERT(emulatedApplication != nullptr);

    EmulatedApplication * registeredEmulatedApplication = findByMnemonic(emulatedApplication->mnemonic());
    if (registeredEmulatedApplication == nullptr)
    {
        _registry.append(emulatedApplication);
        return true;
    }
    return registeredEmulatedApplication == emulatedApplication;
}

//  End of emuone-scp360/EmulatedApplication.cpp
