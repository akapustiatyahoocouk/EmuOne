//
//  emuone-util/Iso88591CharacterSet.cpp
//
//  The util::Iso88591CharacterSet class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Iso88591CharacterSet)
Iso88591CharacterSet::Iso88591CharacterSet() {}
Iso88591CharacterSet::~Iso88591CharacterSet() {}

//////////
//  StockObject
QString Iso88591CharacterSet::mnemonic() const
{
    return "ISO/IEC 8859-1:1998";
}

QString Iso88591CharacterSet::displayName() const
{
    return "ISO-8859-1";
}

//////////
//  CharacterSet
int Iso88591CharacterSet::minimumEncodingLength() const
{
    return 1;
}

int Iso88591CharacterSet::maximumEncodingLength() const
{
    return 1;
}

CharacterSet::Encoder * Iso88591CharacterSet::createEncoder()
{
    return new _Encoder();
}

CharacterSet::Decoder * Iso88591CharacterSet::createDecoder()
{
    return new _Decoder();
}

//////////
//  Iso88591CharacterSet::_Encoder
CharacterSet * Iso88591CharacterSet::_Encoder::characterSet() const
{
    return Iso88591CharacterSet::getInstance();
}

void Iso88591CharacterSet::_Encoder::reset()
{   //  ISO-8859-1 is stateless
}

int Iso88591CharacterSet::_Encoder::encodingLength(QChar c)
{
    return (c.unicode() <= 255) ? 1 : 0;
}

int Iso88591CharacterSet::_Encoder::encode(QChar c, QByteArray & bytes)
{
    if (c.unicode() <= 255)
    {
        bytes.append((uint8_t)c.unicode());
        return 1;
    }
    return 0;
}

int Iso88591CharacterSet::_Encoder::encode(const QString & s, QByteArray & bytes)
{
    int result = 0;
    for (QChar c : s)
    {
        result += encode(c, bytes);
    }
    return result;
}

//////////
//  Iso88591CharacterSet::_Decoder
CharacterSet * Iso88591CharacterSet::_Decoder::characterSet() const
{
    return Iso88591CharacterSet::getInstance();
}

void Iso88591CharacterSet::_Decoder::reset()
{   //  ISO-8859-1 is stateless
}

int Iso88591CharacterSet::_Decoder::encodingLength(const QByteArray & bytes, int scan)
{
    return (scan >= 0 && scan < bytes.length()) ? 1 : 0;
}

int Iso88591CharacterSet::_Decoder::decode(const QByteArray & bytes, int scan, QChar & c)
{
    if (scan >= 0 && scan < bytes.length())
    {
        c = bytes[scan];
        return 1;
    }
    return 0;
}

int Iso88591CharacterSet::_Decoder::decode(const QByteArray & bytes, int scan, QString & s)
{
    int result = 0;
    s.clear();
    for (; ; )
    {
        QChar c;
        int count = decode(bytes, scan, c);
        if (count == 0)
        {   //  No more!
            return result;
        }
        else
        {   //  We have 1 more character
            result += count;
            scan += count;
            s += c;
        }
    }
}

//  End of emuone-util/Iso88591CharacterSet.cpp
