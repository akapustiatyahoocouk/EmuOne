//
//  emuone-util/CharacterSet.cpp
//
//  The util::CharacterSet class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

CharacterSetList CharacterSet::_registry;

//////////
//  Construction/destruction
CharacterSet::CharacterSet()
{
}

CharacterSet::~CharacterSet()
{
}

//////////
//  StockObject
QIcon CharacterSet::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/util/Images/CharacterSetSmall.png");
    }
    return _smallIcon;
}

QIcon CharacterSet::largeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/util/Images/CharacterSetLarge.png");
    }
    return _largeIcon;
}

//////////
//  Registry
CharacterSetList CharacterSet::all()
{
    return _registry;
}

CharacterSet * CharacterSet::findByMnemonic(const QString & mnemonic)
{
    for (CharacterSet * characterSet : _registry)
    {
        if (characterSet->mnemonic() == mnemonic)
        {
            return characterSet;
        }
    }
    return nullptr;
}

bool CharacterSet::registerCharacterSet(CharacterSet * characterSet)
{
    Q_ASSERT(characterSet != nullptr);

    CharacterSet * registeredCharacterSet = findByMnemonic(characterSet->mnemonic());
    if (registeredCharacterSet == nullptr)
    {
        _registry.append(characterSet);
        return true;
    }
    return registeredCharacterSet == characterSet;
}

//  End of emuone-util/CharacterSet.cpp
