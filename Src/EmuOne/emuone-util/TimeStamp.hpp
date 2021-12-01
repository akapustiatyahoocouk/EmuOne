//
//  emuone-util/TimeStamp.hpp
//
//  "time stamp" ADT
//
//////////

namespace util
{
    //////////
    //  The number of nanoseconds since (>0) or before (<0) the Unix epoch
    //  (1 Jan 1970, 00:00:00); always expressed in UTC.
    //  In addition, a special "invalid" time stamp can be represented, which
    //  compares "less than" any valid time stamp
    class EMUONE_UTIL_EXPORT TimeStamp
    {
        //////////
        //  Constants
    private:
        static const int64_t    _InvalidValue = INT64_MIN;
    public:
        //  An "invalid" time stamp
        static const TimeStamp  Invalid;

        //////////
        //  Construction/destruction/assignment
    private:
        explicit TimeStamp(int64_t value) : _value(value) {}
    public:
        TimeStamp() : _value(_InvalidValue) {}

        //////////
        //  Operators
    public:
        //  Compares two time stamps
        bool                operator == (const TimeStamp & op2) const { return _value == op2._value; }
        bool                operator != (const TimeStamp & op2) const { return _value != op2._value; }
        bool                operator <  (const TimeStamp & op2) const { return _value <  op2._value; }
        bool                operator <= (const TimeStamp & op2) const { return _value <= op2._value; }
        bool                operator >  (const TimeStamp & op2) const { return _value >  op2._value; }
        bool                operator >= (const TimeStamp & op2) const { return _value >= op2._value; }

        //  Returns the difference between two time stamps
        TimeSpan            operator - (const TimeStamp & op2) const;

        //////////
        //  Operations
    public:
        //  Checks if this stamp is "valid"
        bool                isValid() const { return _value != _InvalidValue; }

        //  Compares two time stamps, returning a negative, zero or positive
        //  value depending on whether the 1st time stamp is "less than", "equal
        //  to" or "greater than" the 2nd one
        int                 compareTo(const TimeStamp & op2) const;

        //  Returns the current moment.
        static TimeStamp    now();

        //////////
        //  Implementation
    private:
        int64_t             _value;
    };
}

//  End of emuone-util/TimeStamp.hpp
