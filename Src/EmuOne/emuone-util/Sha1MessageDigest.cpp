//
//  emuone-util/Sha1MessageDigest.cpp
//
//  The util::Sha1MessageDigest class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Sha1MessageDigest)
Sha1MessageDigest::Sha1MessageDigest() {}
Sha1MessageDigest::~Sha1MessageDigest() {}

//////////
//  StockObject
QString Sha1MessageDigest::mnemonic() const
{
    return "SHA-1";
}

QString Sha1MessageDigest::displayName() const
{
    return "SHA-1";
}

//////////
//  MessageDigest
MessageDigest::Builder * Sha1MessageDigest::createBuilder()
{
    return new _Builder();
}

//////////
//  Sha1MessageDigest::_Builder
Sha1MessageDigest::_Builder::_Builder()
{
    reset();
}

Sha1MessageDigest::_Builder::~_Builder()
{
}

MessageDigest * Sha1MessageDigest::_Builder::messageDigest() const
{
    return Sha1MessageDigest::instance();
}

void Sha1MessageDigest::_Builder::reset()
{
    _lengthLo = 0;
    _lengthHi = 0;
    _messageBlockIndex = 0;

    _H[0] = 0x67452301;
    _H[1] = 0xEFCDAB89;
    _H[2] = 0x98BADCFE;
    _H[3] = 0x10325476;
    _H[4] = 0xC3D2E1F0;

    _result.clear();
    _finalised = false;
}

void Sha1MessageDigest::_Builder::digestBytes(const void * data, size_t numBytes)
{
    Q_ASSERT(!_finalised);

    Q_ASSERT(_result.isEmpty());
    Q_ASSERT(data != nullptr);
    const uint8_t * bytes = static_cast<const uint8_t *>(data);

    if (numBytes == 0)
    {
        return;
    }

    while (numBytes-- != 0)
    {
        _messageBlock[_messageBlockIndex++] = *bytes;

        _lengthLo += 8;
        _lengthLo &= 0xFFFFFFFF;               // Force it to 32 bits
        if (_lengthLo == 0)
        {
            _lengthHi++;
        }

        if (_messageBlockIndex == 64)
        {
            _processMessageBlock();
        }

        bytes++;
    }
}

void Sha1MessageDigest::_Builder::finalise()
{
    Q_ASSERT(!_finalised);

    _padMessage();
    _result.resize(20);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            _result[4 * i + j] = static_cast<uint8_t>(_H[i] >> ((3 - j) * 8));
        }
    }
    _finalised = true;
}

QByteArray Sha1MessageDigest::_Builder::digest()
{
    if (!_finalised)
    {
        finalise();
    }

    Q_ASSERT(!_result.isEmpty());
    return _result;
}

void Sha1MessageDigest::_Builder::_processMessageBlock()
{
    static const uint32_t K[] =
        {   // Constants defined for SHA-1
            0x5A827999,
            0x6ED9EBA1,
            0x8F1BBCDC,
            0xCA62C1D6
        };

    uint32_t    temp;             //  Temporary word value
    uint32_t    W[80];            //  Word sequence
    uint32_t    A, B, C, D, E;    //  Word buffers

    //  Initialize the first 16 words in the array W
    for (int t = 0; t < 16; t++)
    {
        W[t]  = static_cast<uint32_t>(_messageBlock[t * 4 ]) << 24;
        W[t] |= static_cast<uint32_t>(_messageBlock[t * 4 + 1]) << 16;
        W[t] |= static_cast<uint32_t>(_messageBlock[t * 4 + 2]) << 8;
        W[t] |= static_cast<uint32_t>(_messageBlock[t * 4 + 3]);
    }

    for (int t = 16; t < 80; t++)
    {
       W[t] = _circularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = _H[0];
    B = _H[1];
    C = _H[2];
    D = _H[3];
    E = _H[4];

    for (int t = 0; t < 20; t++)
    {
        temp = _circularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for (int t = 20; t < 40; t++)
    {
        temp = _circularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for (int t = 40; t < 60; t++)
    {
        temp = _circularShift(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for (int t = 60; t < 80; t++)
    {
        temp = _circularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    _H[0] = (_H[0] + A) & 0xFFFFFFFF;
    _H[1] = (_H[1] + B) & 0xFFFFFFFF;
    _H[2] = (_H[2] + C) & 0xFFFFFFFF;
    _H[3] = (_H[3] + D) & 0xFFFFFFFF;
    _H[4] = (_H[4] + E) & 0xFFFFFFFF;

    _messageBlockIndex = 0;
}

void Sha1MessageDigest::_Builder::_padMessage()
{
    //  Check to see if the current message block is too small to hold
    //  the initial padding bits and length.  If so, we will pad the
    //  block, process it, and then continue padding into a second block.
    if (_messageBlockIndex > 55)
    {
        _messageBlock[_messageBlockIndex++] = 0x80;
        while (_messageBlockIndex < 64)
        {
            _messageBlock[_messageBlockIndex++] = 0;
        }

        _processMessageBlock();

        while (_messageBlockIndex < 56)
        {
            _messageBlock[_messageBlockIndex++] = 0;
        }
    }
    else
    {
        _messageBlock[_messageBlockIndex++] = 0x80;
        while (_messageBlockIndex < 56)
        {
            _messageBlock[_messageBlockIndex++] = 0;
        }
    }

    //  Store the message length as the last 8 octets
    _messageBlock[56] = static_cast<uint8_t>(_lengthHi >> 24);
    _messageBlock[57] = static_cast<uint8_t>(_lengthHi >> 16);
    _messageBlock[58] = static_cast<uint8_t>(_lengthHi >> 8);
    _messageBlock[59] = static_cast<uint8_t>(_lengthHi);
    _messageBlock[60] = static_cast<uint8_t>(_lengthLo >> 24);
    _messageBlock[61] = static_cast<uint8_t>(_lengthLo >> 16);
    _messageBlock[62] = static_cast<uint8_t>(_lengthLo >> 8);
    _messageBlock[63] = static_cast<uint8_t>(_lengthLo);

    _processMessageBlock();
}

//  End of emuone-util/Sha1MessageDigest.cpp
