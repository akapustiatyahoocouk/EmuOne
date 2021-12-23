//
//  emuone-util/ToString.hpp
//
//  Value formatting framework.
//
//////////

namespace util
{
    //  Converts the specified "value" to a string. The goal is not to create
    //  a human-readable repreesentation, but something re-parsable that
    //  "fromString()" can recover later.
    EMUONE_UTIL_EXPORT QString toString(bool value);
    EMUONE_UTIL_EXPORT QString toString(char value);
    EMUONE_UTIL_EXPORT QString toString(unsigned char value);
    EMUONE_UTIL_EXPORT QString toString(signed char value);
    EMUONE_UTIL_EXPORT QString toString(unsigned short value);
    EMUONE_UTIL_EXPORT QString toString(signed short value);
    EMUONE_UTIL_EXPORT QString toString(unsigned int value);
    EMUONE_UTIL_EXPORT QString toString(signed int value);
    EMUONE_UTIL_EXPORT QString toString(unsigned long value);
    EMUONE_UTIL_EXPORT QString toString(signed long value);
    EMUONE_UTIL_EXPORT QString toString(unsigned long long value);
    EMUONE_UTIL_EXPORT QString toString(signed long long value);
    EMUONE_UTIL_EXPORT QString toString(float value);
    EMUONE_UTIL_EXPORT QString toString(double value);
    EMUONE_UTIL_EXPORT QString toString(const QString & value);

    //  Converts the specified "value" to a string using a CRT-style
    //  format specifier (e.g.; "%d" for ints, etc.)
    //  The data type prefixes (as in "ll" in "%lld" or "%016lX") should NOT
    //  be used; the framework takes care of that.
    EMUONE_UTIL_EXPORT QString toString(unsigned char value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(signed char value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(unsigned short value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(signed short value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(unsigned int value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(signed int value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(unsigned long value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(signed long value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(unsigned long long value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(signed long long value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(float value, const char * crtFormat);
    EMUONE_UTIL_EXPORT QString toString(double value, const char * crtFormat);
}

//  End of emuone-util/ToString.hpp
