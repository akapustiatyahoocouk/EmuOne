//
//  emuone-util/Memory.cpp
//
//  The util::Memory class implementation + support
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

//////////
//  Formatting/parsing
EMUONE_UTIL_EXPORT QString util::toString(ByteOrder value)
{
    return (value == ByteOrder::BigEndian) ? "BigEndian" : "LittleEndian";
}

EMUONE_UTIL_EXPORT bool util::fromString(const QString & s, qsizetype & scan, ByteOrder & value)
{
    if (scan < s.length() && s.mid(scan).toLower().startsWith("bigendian"))
    {
        value = ByteOrder::BigEndian;
        scan += 9;
        return true;
    }
    if (scan < s.length() && s.mid(scan).toLower().startsWith("littleendian"))
    {
        value = ByteOrder::LittleEndian;
        scan += 12;
        return true;
    }
    return false;
}

EMUONE_UTIL_EXPORT bool util::fromString(const QString & s, ByteOrder & value)
{
    ByteOrder oldValue = value;
    qsizetype scan = 0;
    if (fromString(s, scan, value) && scan == s.length())
    {
        return true;
    }
    //  OOPS! Restore "value" before returning
    value = oldValue;
    return false;
}

//  End of emuone-util/Memory.cpp
