//
//  emuone-util/TimeSpan.hpp
//
//  "time span" ADT
//
//////////

namespace util
{
    //////////
    //  A difference, in nanoseconds between two moments in time.
    //  In addition, a special "invalid" time span can be represented, which
    //  compares "less than" any valid time span
    class EMUONE_UTIL_EXPORT TimeSpan
    {
        //////////
        //  Constants
    private:
        static const int64_t    _InvalidValue = INT64_MIN;
        static const int64_t    _NanosecondsPerMicrosecond = 1000;
        static const int64_t    _NanosecondsPerMillisecond = _NanosecondsPerMicrosecond * 1000;
        static const int64_t    _NanosecondsPerSecond = _NanosecondsPerMillisecond * 1000;
        static const int64_t    _NanosecondsPerMinute = _NanosecondsPerSecond * 60;
        static const int64_t    _NanosecondsPerHour = _NanosecondsPerMinute * 60;
        static const int64_t    _NanosecondsPerDay = _NanosecondsPerHour * 24;
    public:
        //  An "invalid" time span
        static const TimeSpan   Invalid;

        //////////
        //  Construction/destruction/assignment
    private:
        explicit TimeSpan(int64_t value) : _value(value) {}
    public:
        TimeSpan() : _value(_InvalidValue) {}

        //////////
        //  Operators
    public:
        //  Compares two time spans
        bool                operator == (const TimeSpan & op2) const { return _value == op2._value; }
        bool                operator != (const TimeSpan & op2) const { return _value != op2._value; }
        bool                operator <  (const TimeSpan & op2) const { return _value <  op2._value; }
        bool                operator <= (const TimeSpan & op2) const { return _value <= op2._value; }
        bool                operator >  (const TimeSpan & op2) const { return _value >  op2._value; }
        bool                operator >= (const TimeSpan & op2) const { return _value >= op2._value; }

        //////////
        //  Operations
    public:
        //  Constructs a "time span" from the specified number of corresponding units
        static TimeSpan     fromDays(int64_t count);
        static TimeSpan     fromHours(int64_t count);
        static TimeSpan     fromMinutes(int64_t count);
        static TimeSpan     fromSeconds(int64_t count);
        static TimeSpan     fromMilliseconds(int64_t count);
        static TimeSpan     fromMicroseconds(int64_t count);
        static TimeSpan     fromNanoseconds(int64_t count);

        //  Returns this time span expressed in the specified units
        int64_t             asDays() const;
        int64_t             asHours() const;
        int64_t             asMinutes() const;
        int64_t             asSeconds() const;
        int64_t             asMilliseconds() const;
        int64_t             asMicroseconds() const;
        int64_t             asNanoseconds() const;

        //  Checks if this span is "valid"
        bool                isValid() const { return _value != _InvalidValue; }

        //  Compares two time spans, returning a negative, zero or positive
        //  value depending on whether the 1st time span is "less than", "equal
        //  to" or "greater than" the 2nd one
        int                 compareTo(const TimeSpan & op2) const;

        //////////
        //  Implementation
    private:
        int64_t             _value;
    };
}

//  End of emuone-util/TimeSpan.hpp
