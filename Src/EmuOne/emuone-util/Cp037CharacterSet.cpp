//
//  emuone-util/Cp037CharacterSet.cpp
//
//  The util::Cp037CharacterSet class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

const uint8_t Cp037CharacterSet::_ebcdicToAscii[256] =
{
    0x00000000,
    0x00000001,
    0x00000002,
    0x00000003,
    0x0000009C,
    0x00000009,
    0x00000086,
    0x0000007F,
    0x00000097,
    0x0000008D,
    0x0000008E,
    0x0000000B,
    0x0000000C,
    0x0000000D,
    0x0000000E,
    0x0000000F,
    0x00000010,
    0x00000011,
    0x00000012,
    0x00000013,
    0x0000009D,
    0x00000085,
    0x00000008,
    0x00000087,
    0x00000018,
    0x00000019,
    0x00000092,
    0x0000008F,
    0x0000001C,
    0x0000001D,
    0x0000001E,
    0x0000001F,
    0x00000080,
    0x00000081,
    0x00000082,
    0x00000083,
    0x00000084,
    0x0000000A,
    0x00000017,
    0x0000001B,
    0x00000088,
    0x00000089,
    0x0000008A,
    0x0000008B,
    0x0000008C,
    0x00000005,
    0x00000006,
    0x00000007,
    0x00000090,
    0x00000091,
    0x00000016,
    0x00000093,
    0x00000094,
    0x00000095,
    0x00000096,
    0x00000004,
    0x00000098,
    0x00000099,
    0x0000009A,
    0x0000009B,
    0x00000014,
    0x00000015,
    0x0000009E,
    0x0000001A,
    0x00000020,
    0x000000A0,
    0x000000E2,
    0x000000E4,
    0x000000E0,
    0x000000E1,
    0x000000E3,
    0x000000E5,
    0x000000E7,
    0x000000F1,
    0x000000A2,
    0x0000002E,
    0x0000003C,
    0x00000028,
    0x0000002B,
    0x0000007C,
    0x00000026,
    0x000000E9,
    0x000000EA,
    0x000000EB,
    0x000000E8,
    0x000000ED,
    0x000000EE,
    0x000000EF,
    0x000000EC,
    0x000000DF,
    0x00000021,
    0x00000024,
    0x0000002A,
    0x00000029,
    0x0000003B,
    0x000000AC,
    0x0000002D,
    0x0000002F,
    0x000000C2,
    0x000000C4,
    0x000000C0,
    0x000000C1,
    0x000000C3,
    0x000000C5,
    0x000000C7,
    0x000000D1,
    0x000000A6,
    0x0000002C,
    0x00000025,
    0x0000005F,
    0x0000003E,
    0x0000003F,
    0x000000F8,
    0x000000C9,
    0x000000CA,
    0x000000CB,
    0x000000C8,
    0x000000CD,
    0x000000CE,
    0x000000CF,
    0x000000CC,
    0x00000060,
    0x0000003A,
    0x00000023,
    0x00000040,
    0x00000027,
    0x0000003D,
    0x00000022,
    0x000000D8,
    0x00000061,
    0x00000062,
    0x00000063,
    0x00000064,
    0x00000065,
    0x00000066,
    0x00000067,
    0x00000068,
    0x00000069,
    0x000000AB,
    0x000000BB,
    0x000000F0,
    0x000000FD,
    0x000000FE,
    0x000000B1,
    0x000000B0,
    0x0000006A,
    0x0000006B,
    0x0000006C,
    0x0000006D,
    0x0000006E,
    0x0000006F,
    0x00000070,
    0x00000071,
    0x00000072,
    0x000000AA,
    0x000000BA,
    0x000000E6,
    0x000000B8,
    0x000000C6,
    0x000000A4,
    0x000000B5,
    0x0000007E,
    0x00000073,
    0x00000074,
    0x00000075,
    0x00000076,
    0x00000077,
    0x00000078,
    0x00000079,
    0x0000007A,
    0x000000A1,
    0x000000BF,
    0x000000D0,
    0x000000DD,
    0x000000DE,
    0x000000AE,
    0x0000005E,
    0x000000A3,
    0x000000A5,
    0x000000B7,
    0x000000A9,
    0x000000A7,
    0x000000B6,
    0x000000BC,
    0x000000BD,
    0x000000BE,
    0x0000005B,
    0x0000005D,
    0x000000AF,
    0x000000A8,
    0x000000B4,
    0x000000D7,
    0x0000007B,
    0x00000041,
    0x00000042,
    0x00000043,
    0x00000044,
    0x00000045,
    0x00000046,
    0x00000047,
    0x00000048,
    0x00000049,
    0x000000AD,
    0x000000F4,
    0x000000F6,
    0x000000F2,
    0x000000F3,
    0x000000F5,
    0x0000007D,
    0x0000004A,
    0x0000004B,
    0x0000004C,
    0x0000004D,
    0x0000004E,
    0x0000004F,
    0x00000050,
    0x00000051,
    0x00000052,
    0x000000B9,
    0x000000FB,
    0x000000FC,
    0x000000F9,
    0x000000FA,
    0x000000FF,
    0x0000005C,
    0x000000F7,
    0x00000053,
    0x00000054,
    0x00000055,
    0x00000056,
    0x00000057,
    0x00000058,
    0x00000059,
    0x0000005A,
    0x000000B2,
    0x000000D4,
    0x000000D6,
    0x000000D2,
    0x000000D3,
    0x000000D5,
    0x00000030,
    0x00000031,
    0x00000032,
    0x00000033,
    0x00000034,
    0x00000035,
    0x00000036,
    0x00000037,
    0x00000038,
    0x00000039,
    0x000000B3,
    0x000000DB,
    0x000000DC,
    0x000000D9,
    0x000000DA,
    0x0000009F
};

uint8_t Cp037CharacterSet::_asciiToEbcdic[256];

//////////
//  Singleton
IMPLEMENT_SINGLETON(Cp037CharacterSet)
Cp037CharacterSet::Cp037CharacterSet()
{
    for (int i = 0; i < 256; i++)
    {
        _asciiToEbcdic[_ebcdicToAscii[i]] = static_cast<uint8_t>(i);
    }
}
Cp037CharacterSet::~Cp037CharacterSet() {}

//////////
//  StockObject
QString Cp037CharacterSet::mnemonic() const
{
    return "CP037";
}

QString Cp037CharacterSet::displayName() const
{
    return "EBCDIC (US)";
}

//////////
//  CharacterSet
int Cp037CharacterSet::minimumEncodingLength() const
{
    return 1;
}

int Cp037CharacterSet::maximumEncodingLength() const
{
    return 1;
}

CharacterSet::Encoder * Cp037CharacterSet::createEncoder()
{
    return new _Encoder();
}

CharacterSet::Decoder * Cp037CharacterSet::createDecoder()
{
    return new _Decoder();
}

//////////
//  Cp037CharacterSet::_Encoder
CharacterSet * Cp037CharacterSet::_Encoder::characterSet() const
{
    return Cp037CharacterSet::instance();
}

void Cp037CharacterSet::_Encoder::reset()
{   //  EBCDIC is stateless
}

int Cp037CharacterSet::_Encoder::encodingLength(QChar c)
{
    return (c.unicode() <= 255) ? 1 : 0;
}

int Cp037CharacterSet::_Encoder::encode(QChar c, QByteArray & bytes)
{
    if (c.unicode() <= 255)
    {
        bytes.append(_asciiToEbcdic[c.unicode()]);
        return 1;
    }
    return 0;
}

int Cp037CharacterSet::_Encoder::encode(const QString & s, QByteArray & bytes)
{
    int result = 0;
    for (QChar c : s)
    {
        result += encode(c, bytes);
    }
    return result;
}

//////////
//  Cp037CharacterSet::_Decoder
CharacterSet * Cp037CharacterSet::_Decoder::characterSet() const
{
    return Cp037CharacterSet::instance();
}

void Cp037CharacterSet::_Decoder::reset()
{   //  EBCDIC is stateless
}

int Cp037CharacterSet::_Decoder::encodingLength(const QByteArray & bytes, int scan)
{
    return (scan >= 0 && scan < bytes.length()) ? 1 : 0;
}

int Cp037CharacterSet::_Decoder::decode(const QByteArray & bytes, int scan, QChar & c)
{
    if (scan >= 0 && scan < bytes.length())
    {
        c = static_cast<char16_t>(_ebcdicToAscii[static_cast<uint8_t>(bytes[scan])]);
        return 1;
    }
    return 0;
}

int Cp037CharacterSet::_Decoder::decode(const QByteArray & bytes, int scan, QString & s)
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

//  End of emuone-util/Cp037CharacterSet.cpp
