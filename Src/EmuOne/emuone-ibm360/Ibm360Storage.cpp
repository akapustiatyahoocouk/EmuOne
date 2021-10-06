//
//  emuone-360/Ibm360Storage.cpp
//
//  Ibm360Storage class implementation
//
//////////
#include "emuone-ibm360/API.hpp"

//////////
//  Construction/destruction
Ibm360Storage::Ibm360Storage(const QString & name, uint32_t size)
    :   Component(name),
        _size(size)
{
}

Ibm360Storage::~Ibm360Storage()
{
}

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

Ibm360Storage * Ibm360Storage::Type::createComponent()
{
    return new Ibm360Storage("Main storage", 128 * 1024);
}

//  End of emuone-360/Ibm360Storage.cpp
