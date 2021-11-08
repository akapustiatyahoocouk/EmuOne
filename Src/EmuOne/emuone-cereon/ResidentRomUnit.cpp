//
//  emuone-cereon/ResidentRomUnit.cpp
//
//  cereon::ResidentRomUnit class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
ResidentRomUnit::ResidentRomUnit()
    :   core::ResidentRomUnit64("ROM", UINT64_C(0xFFFFFFFFFFF00000), core::MemorySize(core::MemorySize::Unit::MB, 1), "rom.bin")
{
}

ResidentRomUnit::~ResidentRomUnit()
{
}

//////////
//  core::Component
core::ComponentType * ResidentRomUnit::type() const
{
    return Type::instance();
}

//////////
//  ResidentRomUnit::Type
IMPLEMENT_SINGLETON(ResidentRomUnit::Type)
ResidentRomUnit::Type::Type() {}
ResidentRomUnit::Type::~Type() {}

QString ResidentRomUnit::Type::mnemonic() const
{
    return "Cereon-ResidentRomUnit";
}

QString ResidentRomUnit::Type::displayName() const
{
    return "Cereon resident ROM unit";
}

core::ComponentCategory * ResidentRomUnit::Type::category() const
{
    return core::StandardComponentCategories::Memory::instance();
}

bool ResidentRomUnit::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return dynamic_cast<CereonArchitecture*>(architecture) != nullptr;
}

core::Component * ResidentRomUnit::Type::createComponent()
{
    return new ResidentRomUnit();
}

//  End of emuone-cereon/ResidentRomUnit.cpp
