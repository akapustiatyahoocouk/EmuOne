//
//  emuone-cereon/ResidentRamUnit.cpp
//
//  cereon::ResidentRamUnit class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
ResidentRamUnit::ResidentRamUnit()
    :   core::StandardResidentRamUnit64("RAM", 0, core::MemorySize(core::MemorySize::Unit::MB, 64))
{
}

ResidentRamUnit::~ResidentRamUnit()
{
}

//////////
//  core::Component
core::ComponentType * ResidentRamUnit::type() const
{
    return Type::instance();
}

//////////
//  ResidentRamUnit::Type
IMPLEMENT_SINGLETON(ResidentRamUnit::Type)
ResidentRamUnit::Type::Type() {}
ResidentRamUnit::Type::~Type() {}

QString ResidentRamUnit::Type::mnemonic() const
{
    return "Cereon-ResidentRamUnit";
}

QString ResidentRamUnit::Type::displayName() const
{
    return "Cereon resident RAM unit";
}

core::ComponentCategory * ResidentRamUnit::Type::category() const
{
    return core::StandardComponentCategories::Memory::instance();
}

bool ResidentRamUnit::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return dynamic_cast<CereonArchitecture*>(architecture) != nullptr;
}

core::Component * ResidentRamUnit::Type::createComponent()
{
    return new ResidentRamUnit();
}

//  End of emuone-cereon/ResidentRamUnit.cpp
