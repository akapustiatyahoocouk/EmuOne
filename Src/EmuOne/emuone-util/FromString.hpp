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
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, bool & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, char & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, unsigned char & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, signed char & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, unsigned short & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, signed short & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, unsigned int & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, signed int & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, unsigned long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, signed long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, unsigned long long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, signed long long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, float & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, double & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, QString & value);

    //  Tries to parse an entire string as a value
    template <class T>
    bool fromString(const QString & s, T & value)
    {
        qsizetype scan = 0;
        T oldValue = value;
        if (util::fromString(s, scan, value) && scan == s.length())
        {
            return true;
        }
        //  OOPS! Restore "value" before returning!
        value = oldValue;
        return false;
    }

    //  Parsing with custom format strngs
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned char & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed char & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned short & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed short & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned int & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed int & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, unsigned long long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, signed long long & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, float & value);
    EMUONE_UTIL_EXPORT bool fromString(const QString & s, qsizetype & scan, const char * crtFormat, double & value);

    //  Tries to parse an entire string as a value with custoim CRT format
    template <class T>
    bool fromString(const QString & s, const char * crtFormat, T & value)
    {
        qsizetype scan = 0;
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
