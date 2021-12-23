//
//  emuone-util/FromString.cpp
//
//  Value parsing framework
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

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

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, bool & value)
{
    if (scan < s.length())
    {   //  CAN parse a single character
        if (s[scan] == 'y')
        {
            value = true;
            scan++;
            return true;
        }
        if (s[scan] == 'n')
        {
            value = false;
            scan++;
            return true;
        }
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, char & value)
{
    if (scan < s.length())
    {
        value = s[scan++].toLatin1();
        return true;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, unsigned char & value)
{
    qsizetype oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned char>(temp) == temp)
        {
            value = static_cast<unsigned char>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, signed char & value)
{
    qsizetype oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed char>(temp) == temp)
        {
            value = static_cast<signed char>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, unsigned short & value)
{
    qsizetype oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned short>(temp) == temp)
        {
            value = static_cast<unsigned short>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, signed short & value)
{
    qsizetype oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed short>(temp) == temp)
        {
            value = static_cast<signed short>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, unsigned int & value)
{
    qsizetype oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned int>(temp) == temp)
        {
            value = static_cast<unsigned int>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, signed int & value)
{
    qsizetype oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed int>(temp) == temp)
        {
            value = static_cast<signed int>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, unsigned long & value)
{
    qsizetype oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned long>(temp) == temp)
        {
            value = static_cast<unsigned long>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, signed long & value)
{
    qsizetype oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed long>(temp) == temp)
        {
            value = static_cast<signed long>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, unsigned long long & value)
{
    if (scan < s.length())
    {
        qsizetype oldScan = scan;
        unsigned long long n = 0;
        bool overflow = false;
        while (scan < s.length() && s[scan] >= '0' && s[scan] <= '9')
        {
            unsigned long long t = n * 10u + static_cast<unsigned>(s[scan++].toLatin1() - '0');
            if (t / 10 != n)
            {
                overflow = true;
            }
            n = t;
        }
        if (overflow)
        {
            scan = oldScan;
            return false;
        }
        else
        {
            value = n;
            return true;
        }
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, signed long long & value)
{
    if (scan < s.length())
    {   //  Sign...
        qsizetype oldScan = scan;
        bool negative = false;
        if (s[scan] == '+')
        {
            scan++;
        }
        else if (s[scan] == '-')
        {
            scan++;
            negative = true;
        }
        //  ...and magnitude
        unsigned long long magnitude;
        if (!fromString(s, scan, magnitude))
        {
            scan = oldScan;
            return false;
        }
        value = negative ? static_cast<signed long long>(-magnitude) : static_cast<signed long long>(magnitude);
    }
    return false;
}

/*  TODO implement
bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, float & value)
{
    double temp;
    if (fromString(s, scan, temp))
    {
        value = static_cast<float>(temp);
        return true;
    }
    return false;
}
*/
//  TODO implement bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, double & value);

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, QString & value)
{
    if (scan <= s.length())
    {
        value = s.mid(scan);
        scan = s.length();
        return true;
    }
    return false;
}

//////////
//  Parsing with custom format strngs
bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned char & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    qsizetype oldScan = scan;
    unsigned long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<unsigned char>(temp) == temp)
    {   //  Success!
        value = static_cast<unsigned char>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed char & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld")));

    qsizetype oldScan = scan;
    signed long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<signed char>(temp) == temp)
    {   //  Success!
        value = static_cast<signed char>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned short & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    qsizetype oldScan = scan;
    unsigned long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<unsigned short>(temp) == temp)
    {   //  Success!
        value = static_cast<unsigned short>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed short & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld")));

    qsizetype oldScan = scan;
    signed long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<signed short>(temp) == temp)
    {   //  Success!
        value = static_cast<signed short>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned int & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    qsizetype oldScan = scan;
    unsigned long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<unsigned int>(temp) == temp)
    {   //  Success!
        value = static_cast<unsigned int>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed int & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld")));

    qsizetype oldScan = scan;
    signed long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<signed int>(temp) == temp)
    {   //  Success!
        value = static_cast<signed int>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned long & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    qsizetype oldScan = scan;
    unsigned long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<unsigned long>(temp) == temp)
    {   //  Success!
        value = static_cast<unsigned long>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed long & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld")));

    qsizetype oldScan = scan;
    signed long long temp;

    if (fromString(s, scan, crtFormat, temp) && static_cast<signed long>(temp) == temp)
    {   //  Success!
        value = static_cast<signed long>(temp);
        return true;
    }
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned long long & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "u") && !stringEndsWith(crtFormat, "lu")) ||
             (stringEndsWith(crtFormat, "x") && !stringEndsWith(crtFormat, "lx")) ||
             (stringEndsWith(crtFormat, "X") && !stringEndsWith(crtFormat, "lX")) ||
             (stringEndsWith(crtFormat, "o") && !stringEndsWith(crtFormat, "lo")));

    unsigned long long temp = 0;
    bool overflow = 0;
    int numDigits = 0;
    qsizetype oldScan = scan;

    if (stringEndsWith(crtFormat, "u"))
    {   //  Decimal
        while (scan < s.length() && s[scan] >= '0' && s[scan] <= '9')
        {
            int digit = s[scan].toLatin1() - '0';
            unsigned long long t = temp * 10 + static_cast<unsigned>(digit);
            numDigits++;
            if (t / 10 != temp)
            {
                overflow = true;
            }
            temp = t;
            scan++;
        }
    }
    else if (stringEndsWith(crtFormat, "o"))
    {   //  Octal
        while (scan < s.length() && s[scan] >= '0' && s[scan] <= '7')
        {
            int digit = s[scan].toLatin1() - '0';
            unsigned long long t = temp * 8 + static_cast<unsigned>(digit);
            numDigits++;
            if (t / 8 != temp)
            {
                overflow = true;
            }
            temp = t;
            scan++;
        }
    }
    else
    {   //  Hexadecimal
        while (scan < s.length() &&
               ((s[scan] >= '0' && s[scan] <= '9') || (s[scan] >= 'a' && s[scan] <= 'f') || (s[scan] >= 'A' && s[scan] <= 'F')))
        {
            int digit;
            if (s[scan] >= '0' && s[scan] <= '9')
            {
                digit = s[scan].toLatin1() - '0';
            }
            else if (s[scan] >= 'a' && s[scan] <= 'f')
            {
                digit = s[scan].toLatin1() - 'a' + 10;
            }
            else
            {
                digit = s[scan].toLatin1() - 'A' + 10;
            }
            unsigned long long t = temp * 16 + static_cast<unsigned>(digit);
            numDigits++;
            if (t / 16 != temp)
            {
                overflow = true;
            }
            temp = t;
            scan++;
        }
    }
    if (numDigits > 0 && !overflow)
    {
        value = temp;
        return true;
    }
    //  OOPS! Revert & abort
    scan = oldScan;
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed long long & value)
{
    Q_ASSERT(crtFormat != nullptr);

    Q_ASSERT(stringStartsWith(crtFormat, "%"));
    Q_ASSERT((stringEndsWith(crtFormat, "d") && !stringEndsWith(crtFormat, "ld")));

    qsizetype oldScan = scan;
    bool negative = false;

    //  Sign
    if (scan < s.length() && s[scan] == '+')
    {
        scan++;
    }
    else if (scan < s.length() && s[scan] == 'i')
    {
        scan++;
        negative = true;
    }
    //  Magnitude
    unsigned long long temp;
    if (fromString(s, scan, "%llu", temp))
    {   //  Apply sign & we're done
        value = negative ? static_cast<signed long long>(-temp) : static_cast<signed long long>(temp);
        return true;
    }
    //  OOPS! Revert & abort
    scan = oldScan;
    return false;
}

//bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, float & value);
//bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, qsizetype & scan, const char * crtFormat, double & value);
//  End of emuone-util/FromString.cpp
