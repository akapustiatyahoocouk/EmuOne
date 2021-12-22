//
//  emuone-util/FromString.hpp
//
//  Value parsing framework.
//
//////////

namespace util
{
    //  If a "value" of the proper type can be parsed starting at position "scan" in "s",
    //  then stores the parsed "value", advances the "scan" past the parsed portion of "s"
    //  and retuens "true"; else leaves the "scan" and "value" unchanged and returns "false".
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, bool & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, char & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, unsigned char & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, signed char & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, unsigned short & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, signed short & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, unsigned int & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, signed int & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, unsigned long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, signed long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, unsigned long long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, signed long long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, float & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, double & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, QString & value);

    //  Tries to parse an entire string as a value
    template <class T>
    bool fromString(const QString & s, T & value)
    {
        int scan = 0;
        T oldValue = value;
        if (fromString(s, scan, value) && scan == s.length())
        {
            return true;
        }
        //  OOPS! Restore "value" before returning!
        value = oldValue;
        return false;
    }

    //  Parsing with custom format strngs
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, unsigned char & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, signed char & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, unsigned short & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, signed short & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, unsigned int & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, signed int & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, unsigned long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, signed long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, unsigned long long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, signed long long & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, float & value);
    bool EMUONE_UTIL_EXPORT fromString(const QString & s, int & scan, const char * crtFormat, double & value);

    //  Tries to parse an entire string as a value with custoim CRT format
    template <class T>
    bool fromString(const QString & s, const char * crtFormat, T & value)
    {
        int scan = 0;
        T oldValue = value;
        if (fromString(s, scan, crtFormat, value) && scan == s.length())
        {
            return true;
        }
        //  OOPS! Restore "value" before returning!
        value = oldValue;
        return false;
    }
}

//  End of emuone-util/FromString.hpp
