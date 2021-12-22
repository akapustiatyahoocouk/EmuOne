//
//  emuone-util/ToString.hpp
//
//  Value formatting framework.
//
//////////

namespace util
{
    QString EMUONE_UTIL_EXPORT toString(bool value);
    QString EMUONE_UTIL_EXPORT toString(char value);
    QString EMUONE_UTIL_EXPORT toString(unsigned char value);
    QString EMUONE_UTIL_EXPORT toString(signed char value);
    QString EMUONE_UTIL_EXPORT toString(unsigned short value);
    QString EMUONE_UTIL_EXPORT toString(signed short value);
    QString EMUONE_UTIL_EXPORT toString(unsigned int value);
    QString EMUONE_UTIL_EXPORT toString(signed int value);
    QString EMUONE_UTIL_EXPORT toString(unsigned long value);
    QString EMUONE_UTIL_EXPORT toString(signed long value);
    QString EMUONE_UTIL_EXPORT toString(unsigned long long value);
    QString EMUONE_UTIL_EXPORT toString(signed long long value);
    QString EMUONE_UTIL_EXPORT toString(float value);
    QString EMUONE_UTIL_EXPORT toString(double value);
    QString EMUONE_UTIL_EXPORT toString(const QString & value);

    QString EMUONE_UTIL_EXPORT toString(unsigned char value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(signed char value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(unsigned short value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(signed short value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(unsigned int value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(signed int value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(unsigned long value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(signed long value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(unsigned long long value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(signed long long value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(float value, const char * crtFormat);
    QString EMUONE_UTIL_EXPORT toString(double value, const char * crtFormat);
}

//  End of emuone-util/ToString.hpp
