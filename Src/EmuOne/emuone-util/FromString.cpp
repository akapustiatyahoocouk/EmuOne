//
//  emuone-util/FromString.cpp
//
//  Value parsing framework
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, bool & value)
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

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, char & value)
{
    if (scan < s.length())
    {
        value = s[scan++].toLatin1();
        return true;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, unsigned char & value)
{
    int oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned char>(temp) == value)
        {
            value = static_cast<unsigned char>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, signed char & value)
{
    int oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed char>(temp) == value)
        {
            value = static_cast<signed char>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, unsigned short & value)
{
    int oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned short>(temp) == value)
        {
            value = static_cast<unsigned short>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, signed short & value)
{
    int oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed short>(temp) == value)
        {
            value = static_cast<signed short>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, unsigned int & value)
{
    int oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned int>(temp) == value)
        {
            value = static_cast<unsigned int>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, signed int & value)
{
    int oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed int>(temp) == value)
        {
            value = static_cast<signed int>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, unsigned long & value)
{
    int oldScan = scan;
    unsigned long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<unsigned long>(temp) == value)
        {
            value = static_cast<unsigned long>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, signed long & value)
{
    int oldScan = scan;
    signed long long temp;
    if (fromString(s, scan, temp))
    {   //  Parsed OK, but watch for overflow
        if (static_cast<signed long>(temp) == value)
        {
            value = static_cast<signed long>(temp);
            return true;
        }
        scan = oldScan;
        return false;
    }
    return false;
}

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, unsigned long long & value)
{
    if (scan < s.length())
    {
        int oldScan = scan;
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

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, signed long long & value)
{
    if (scan < s.length())
    {   //  Sign...
        int oldScan = scan;
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

bool EMUONE_UTIL_EXPORT util::fromString(const QString & s, int & scan, QString & value)
{
    if (scan <= s.length())
    {
        value = s.mid(scan);
        scan = static_cast<int>(s.length());
        return true;
    }
    return false;
}

//  End of emuone-util/FromString.cpp
