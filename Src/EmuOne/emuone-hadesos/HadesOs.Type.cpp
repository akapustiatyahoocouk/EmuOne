//
//  emuone-hadesos/HadesOs.Type.cpp
//
//  The hadesos::HadesOs::Type class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos;

//////////
//  Singleton
IMPLEMENT_SINGLETON(HadesOs::Type)
HadesOs::Type::Type() {}
HadesOs::Type::~Type() {}

//////////
//  util::StockObject
QString HadesOs::Type::mnemonic() const
{
    return "HADES-OS";
}

QString HadesOs::Type::displayName() const
{
    return "HADES OS";
}

core::ComponentCategory * HadesOs::Type::category() const
{
    return core::StandardComponentCategories::Software::instance();
}

bool HadesOs::Type::isCompatibleWith(core::Architecture * /*architecture*/) const
{
    return false;   //  rely on adapters!
}

HadesOs * HadesOs::Type::createComponent()
{
    return new HadesOs("HADES OS");
}

//  End of emuone-hadesos/HadesOs.Type.cpp
