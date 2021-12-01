//
//  emuone-util/TimeStamp.cpp
//
//  The util::TimeStamp class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

namespace
{
    inline int64_t _signBit(int64_t x)
    {
        return x & INT64_C(0x8000000000000000);
    }
}

//////////
//  Constants
const TimeStamp TimeStamp::Invalid;

//////////
//  Operators
TimeSpan TimeStamp::operator - (const TimeStamp & op2) const
{
    if (!this->isValid() || !op2.isValid())
    {
        return TimeSpan::Invalid;
    }
    int64_t diffNs = _value - op2._value;
    if (_signBit(_value) != _signBit(op2._value) &&
        _signBit(diffNs) != _signBit(_value))
    {   //  OOPS! Overflow!
        return TimeSpan::Invalid;
    }
    return TimeSpan::fromNanoseconds(diffNs);
}

//////////
//  Operations
int TimeStamp::compareTo(const TimeStamp & op2) const
{
    if (_value < op2._value)
    {
        return -1;
    }
    else if (_value > op2._value)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

TimeStamp TimeStamp::now()
{
    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
    {   //  Success
        return TimeStamp(static_cast<int64_t>(ts.tv_sec) * INT64_C(1000000000) + ts.tv_nsec);
    }
    else
    {   //  Fall back to QT's date/time
        return TimeStamp(QDateTime::currentMSecsSinceEpoch() * 1000000);
    }
}

//  End of emuone-util/TimeStamp.cpp
