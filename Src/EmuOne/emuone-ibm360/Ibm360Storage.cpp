//
//  emuone-360/Ibm360Storage.cpp
//
//  Ibm360Storage class implementation
//
//////////
#include "emuone-ibm360/API.hpp"

//////////
//  Component
ComponentType * Ibm360Storage::getType() const
{
    return Ibm360Storage::Type::getInstance();
}

//////////
//  Ibm360Storage::Type
IMPLEMENT_SINGLETON(Ibm360Storage::Type)
Ibm360Storage::Type::Type() {}
Ibm360Storage::Type::~Type() {}

QString Ibm360Storage::Type::getMnemonic() const
{
    return "Ibm360Storage";
}

QString Ibm360Storage::Type::getDisplayName() const
{
    return "IBM/360 main storage";
}

ComponentCategory * Ibm360Storage::Type::getCategory() const
{
    return StandardComponentCategories::Memory::getInstance();
}

bool Ibm360Storage::Type::isCompatibleWith(Architecture * architecture) const
{
    return architecture == Ibm360Architecture::getInstance();
}

//  End of emuone-360/Ibm360Storage.cpp
