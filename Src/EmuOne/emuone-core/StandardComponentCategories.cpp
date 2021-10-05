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

//////////
//  StandardComponentCategories::Processors
IMPLEMENT_SINGLETON(StandardComponentCategories::Processors)
StandardComponentCategories::Processors::Processors() {}
StandardComponentCategories::Processors::~Processors() {}

QString StandardComponentCategories::Processors::getMnemonic() const
{
    return "Processors";
}

QString StandardComponentCategories::Processors::getDisplayName() const
{
    return "Processors";
}

QIcon StandardComponentCategories::Processors::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/ProcessorsSmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Processors::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/ProcessorsLarge.png");
    }
    return _largeIcon;
}

//////////
//  StandardComponentCategories::Terminals
IMPLEMENT_SINGLETON(StandardComponentCategories::Terminals)
StandardComponentCategories::Terminals::Terminals() {}
StandardComponentCategories::Terminals::~Terminals() {}

QString StandardComponentCategories::Terminals::getMnemonic() const
{
    return "Terminals";
}

QString StandardComponentCategories::Terminals::getDisplayName() const
{
    return "Terminals";
}

QIcon StandardComponentCategories::Terminals::getSmallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/TerminalsSmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Terminals::getLargeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/TerminalsLarge.png");
    }
    return _largeIcon;
}

//  End of emuone-core/StandardComponentCategories.cpp
