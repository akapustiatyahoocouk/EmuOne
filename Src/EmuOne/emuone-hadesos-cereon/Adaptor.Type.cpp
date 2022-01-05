//
//  emuone-hadesos-cereon/Adaptor.Type.cpp
//
//  The emuone::hadesos::Adaptor::Type class implementation
//
//////////
#include "emuone-hadesos-cereon/API.hpp"
using namespace hadesos::cereon;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Adaptor::Type)
Adaptor::Type::Type() {}
Adaptor::Type::~Type() {}

//////////
//  util::StockObject
QString Adaptor::Type::mnemonic() const
{
    return "HadesOS/Cereon";
}

QString Adaptor::Type::displayName() const
{
    return "HADES OS / Cereon adaptor";
}

bool Adaptor::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return dynamic_cast<const ::cereon::CereonArchitecture*>(architecture) != nullptr;
}

bool Adaptor::Type::isCompatibleWith(const core::ComponentType * componentType) const
{
    return dynamic_cast<const hadesos::HadesOs::Type*>(componentType) != nullptr;
}

Adaptor * Adaptor::Type::createAdaptor(core::Component * adaptedComponent)
{
    if (HadesOs * hadesOs = dynamic_cast<HadesOs*>(adaptedComponent))
    {
        return new Adaptor(hadesOs);
    }
    else
    {   //  OOPS! Can't adapt!
        throw core::VirtualApplianceException("Incompatible component");
    }
}

//  End of emuone-hadesos-cereon/Adaptor.Type.cpp
