//
//  emuone-util/MessageDigest.cpp
//
//  The util::MessageDigest class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

MessageDigestList MessageDigest::_registry;

//////////
//  Construction/destruction
MessageDigest::MessageDigest()
{
}

MessageDigest::~MessageDigest()
{
}

//////////
//  StockObject
QIcon MessageDigest::smallIcon() const
{
    if (_smallIcon.isNull())
    {
        _smallIcon = QIcon(":/emuone/util/Images/MessageDigestSmall.png");
    }
    return _smallIcon;
}

QIcon MessageDigest::largeIcon() const
{
    if (_largeIcon.isNull())
    {
        _largeIcon = QIcon(":/emuone/util/Images/MessageDigestLarge.png");
    }
    return _largeIcon;
}

//////////
//  Registry
MessageDigestList MessageDigest::all()
{
    return _registry;
}

MessageDigest * MessageDigest::findByMnemonic(const QString & mnemonic)
{
    for (MessageDigest * messageDigest : _registry)
    {
        if (messageDigest->mnemonic() == mnemonic)
        {
            return messageDigest;
        }
    }
    return nullptr;
}

bool MessageDigest::registerMessageDigest(MessageDigest * messageDigest)
{
    Q_ASSERT(messageDigest != nullptr);

    MessageDigest * registeredMessageDigest = findByMnemonic(messageDigest->mnemonic());
    if (registeredMessageDigest == nullptr)
    {
        _registry.append(messageDigest);
        return true;
    }
    return registeredMessageDigest == messageDigest;
}

//////////
//  MessageDigest::Builder
QString MessageDigest::Builder::digestString()
{
    return this->digest().toHex().toUpper();
}

//  End of emuone-util/MessageDigest.cpp
