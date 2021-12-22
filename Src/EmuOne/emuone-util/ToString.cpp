//
//  emuone-util/ToString.cpp
//
//  Value formatting framework
//
//////////
#include "emuone-util/API.hpp"
using namespace util;
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

namespace
{
    bool stringStartsWith(const char * s, const char * prefix)
    {
        return strlen(s) >= strlen(prefix) && memcmp(s, prefix, strlen(prefix)) == 0;
    }

    bool stringEndsWith(const char * s, const char * suffix)
    {
        size_t sLength = strlen(s);
        size_t suffixLength = strlen(suffix);

        if (sLength < suffixLength)
        {   //  OOPS! Can't match!
            return false;
        }
        return memcmp(s + sLength - suffixLength, suffix, suffixLength) == 0;
    }
}

QString EMUONE_UTIL_EXPORT util::toString(bool value)
{
    return value ? "y" : "n";
}

QString EMUONE_UTIL_EXPORT util::toString(char value)
{
    return QString(QChar(value));
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned char value)
{
    char s[32];
    sprintf(s, "%u", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed char value)
{
    char s[32];
    sprintf(s, "%d", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned short value)
{
    char s[32];
    sprintf(s, "%u", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed short value)
{
    char s[32];
    sprintf(s, "%d", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned int value)
{
    char s[32];
    sprintf(s, "%u", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed int value)
{
    char s[32];
    sprintf(s, "%d", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned long value)
{
    char s[32];
    sprintf(s, "%lu", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed long value)
{
    char s[32];
    sprintf(s, "%ld", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned long long value)
{
    char s[32];
    sprintf(s, "%llu", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed long long value)
{
    char s[32];
    sprintf(s, "%lld", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(float value)
{
    char s[32];
    sprintf(s, "%g", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(double value)
{
    char s[32];
    sprintf(s, "%g", value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(const QString & value)
{
    return value;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned char value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed char value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned short value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed short value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned int value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed int value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned long value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "lu") && !stringEndsWith(crtFormat, "llu")) ||
             (stringEndsWith(crtFormat, "lx") && !stringEndsWith(crtFormat, "llx")) ||
             (stringEndsWith(crtFormat, "lX") && !stringEndsWith(crtFormat, "llX")) ||
             (stringEndsWith(crtFormat, "lo") && !stringEndsWith(crtFormat, "llo")));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed long value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "ld") && !stringEndsWith(crtFormat, "lld"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(unsigned long long value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "llu") ||
             stringEndsWith(crtFormat, "llx") ||
             stringEndsWith(crtFormat, "llX") ||
             stringEndsWith(crtFormat, "llo"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(signed long long value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "lld"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(float value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "f") || stringEndsWith(crtFormat, "e") || stringEndsWith(crtFormat, "g"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

QString EMUONE_UTIL_EXPORT util::toString(double value, const char * crtFormat)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT(stringEndsWith(crtFormat, "f") || stringEndsWith(crtFormat, "e") || stringEndsWith(crtFormat, "g"));

    char s[32];
    sprintf(s, crtFormat, value);
    return s;
}

//  End of emuone-util/ToString.cpp
