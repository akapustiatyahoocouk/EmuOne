//
//  emuone-core/StandardComponentCategories.cpp
//
//  The StandardComponentCategories class implementation
//
//////////
#include "emuone-core/API.hpp"

//////////
//  StandardComponentCategories::Memory
IMPLEMENT_SINGLETON(StandardComponentCategories::Memory)
StandardComponentCategories::Memory::Memory() {}
StandardComponentCategories::Memory::~Memory() {}

QString StandardComponentCategories::Memory::getMnemonic() const
{
    return "Memory";
}

QString StandardComponentCategories::Memory::getDisplayName() const
{
    return "Memory";
}

QIcon StandardComponentCategories::Memory::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/MemorySmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Memory::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/MemoryLarge.png");
    }
    return _largeIcon;
}

//  End of emuone-core/StandardComponentCategories.cpp
