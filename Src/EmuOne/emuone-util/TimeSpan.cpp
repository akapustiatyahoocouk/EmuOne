//
//  emuone-util/TimeSpan.cpp
//
//  The util::TimeSpan class implementation
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

//////////
//  Constants
const TimeSpan TimeSpan::Invalid;

//////////
//  Operations
TimeSpan TimeSpan::fromDays(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerDay;
    if (nanos / _NanosecondsPerDay != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromHours(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerHour;
    if (nanos / _NanosecondsPerHour != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromMinutes(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerMinute;
    if (nanos / _NanosecondsPerMinute != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromSeconds(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerSecond;
    if (nanos / _NanosecondsPerSecond != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromMilliseconds(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerMillisecond;
    if (nanos / _NanosecondsPerMillisecond != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromMicroseconds(int64_t count)
{
    int64_t nanos = count * _NanosecondsPerMicrosecond;
    if (nanos / _NanosecondsPerMicrosecond != count)
    {   //  OOPS! Overflow
        return Invalid;
    }
    return TimeSpan(nanos);
}

TimeSpan TimeSpan::fromNanoseconds(int64_t count)
{
    Q_ASSERT(count != _InvalidValue);

    return TimeSpan(count);
}

int64_t TimeSpan::asDays() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerDay;
}

int64_t TimeSpan::asHours() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerHour;
}

int64_t TimeSpan::asMinutes() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerMinute;
}

int64_t TimeSpan::asSeconds() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerSecond;
}

int64_t TimeSpan::asMilliseconds() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerMillisecond;
}

int64_t TimeSpan::asMicroseconds() const
{
    Q_ASSERT(isValid());

    return _value / _NanosecondsPerMicrosecond;
}

int64_t TimeSpan::asNanoseconds() const
{
    Q_ASSERT(isValid());

    return _value;
}

int TimeSpan::compareTo(const TimeSpan & op2) const
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

//  End of emuone-util/TimeSpan.cpp
