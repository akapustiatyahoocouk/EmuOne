//
//  emuone-core/StandardComponentCategories.cpp
//
//  The StandardComponentCategories class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  StandardComponentCategories::Memory
IMPLEMENT_SINGLETON(StandardComponentCategories::Memory)
StandardComponentCategories::Memory::Memory() {}
StandardComponentCategories::Memory::~Memory() {}

QString StandardComponentCategories::Memory::mnemonic() const
{
    return "Memory";
}

QString StandardComponentCategories::Memory::displayName() const
{
    return "Memory";
}

QIcon StandardComponentCategories::Memory::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/MemorySmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Memory::largeIcon() const
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

QString StandardComponentCategories::Processors::mnemonic() const
{
    return "Processors";
}

QString StandardComponentCategories::Processors::displayName() const
{
    return "Processors";
}

QIcon StandardComponentCategories::Processors::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/ProcessorsSmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Processors::largeIcon() const
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

QString StandardComponentCategories::Terminals::mnemonic() const
{
    return "Terminals";
}

QString StandardComponentCategories::Terminals::displayName() const
{
    return "Terminals";
}

QIcon StandardComponentCategories::Terminals::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/core/Images/TerminalsSmall.png");
    }
    return _smallIcon;
}

QIcon StandardComponentCategories::Terminals::largeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/core/Images/TerminalsLarge.png");
    }
    return _largeIcon;
}

//  End of emuone-core/StandardComponentCategories.cpp
