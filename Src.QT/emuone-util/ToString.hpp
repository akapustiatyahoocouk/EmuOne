//
//  emuone-util/ToString.hpp - Value formatting framework
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::util
{
    template <class T>
    QString toString(const T & value) = delete;

    //  C++ types
    template <> EMUONE_UTIL_PUBLIC QString toString<nullptr_t>(const nullptr_t & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<bool>(const bool & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<char>(const char & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<signed char>(const signed char & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<unsigned char>(const unsigned char & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<signed short>(const signed short & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<unsigned short>(const unsigned short & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<signed int>(const signed int & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<unsigned int>(const unsigned int & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<signed long>(const signed long & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<unsigned long>(const unsigned long & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<signed long long>(const signed long long & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<unsigned long long>(const unsigned long long & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<float>(const float & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<double>(const double & value);
    EMUONE_UTIL_PUBLIC inline      QString toString(const char * value) { return value; }

    //  QT types
    template <> EMUONE_UTIL_PUBLIC QString toString<QChar>(const QChar & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QString>(const QString & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QRect>(const QRect & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QVersionNumber>(const QVersionNumber & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QLocale>(const QLocale & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QColor>(const QColor & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QDateTime>(const QDateTime & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QDate>(const QDate & value);
    template <> EMUONE_UTIL_PUBLIC QString toString<QByteArray>(const QByteArray & value);
}

//  End of emuone-util/ToString.hpp
