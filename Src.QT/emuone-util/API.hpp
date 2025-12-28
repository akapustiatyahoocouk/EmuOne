//
//  emuone-util/API.hpp - emuone-util master header
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
#pragma once

//////////
//  CRT+QT
//  These headers are dirty, so disable some warnings for their processing
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
    #pragma GCC diagnostic ignored "-Wsign-promo"
    #pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
    #pragma GCC diagnostic ignored "-Wswitch-default"
    #pragma GCC diagnostic ignored "-Wconversion"
#elif defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4355)   //  'this': used in base member initializer list
    #pragma warning(disable:4365)   //  '=': conversion from '<TYPE>' to '<TYPE>', signed/unsigned mismatch
    #pragma warning(disable:4371)   //  '<CLASS>': layout of class may have changed from a previous version of the compiler due to better packing of member '<MEMBER>'
    #pragma warning(disable:4437)   //  dynamic_cast from virtual base '<CLASS>' to '<CLASS>' could fail in some contexts
    #pragma warning(disable:4710)   //  '<FUNCTION>': function not inlined
    #pragma warning(disable:4711)   //  '<FUNCTION>': function selected for automatic inline expansion
    #pragma warning(disable:4866)   //  compiler may not enforce left-to-right evaluation order for call to 'C++17 operator'
    #pragma warning(disable:5045)   //  Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    #pragma warning(disable:5219)   //  implicit conversion from 'const int' to 'float', possible loss of data
    #pragma warning(disable:5249)   //  '<CLASS>::<MEMBER>' of type '<TYPE' has named enumerators with values that cannot be represented in the given bit field width of '<WIDTH>'.
#else
    #error Unsupported C++ toolchain
#endif

#include <QtCore/qglobal.h>

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QDomDocument>
#include <QException>
#include <QFileDialog>
#include <QIcon>
#include <QImage>
#include <QLibrary>
#include <QMainWindow>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QPixmap>
#include <QRegularExpression>
#include <QStyleFactory>
#include <QTimer>
#include <QTimeZone>
#include <QVersionNumber>
#include <QSemaphore>
#include <QStackedLayout>
#include <QThread>

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
    //  Some warnings shall be disabled in user code as well
    #pragma warning(disable:4250)   //  '<DERIVED CLASS>': inherits '<BASE MEMBER>' via dominance
    #pragma warning(disable:4251)   //  '<VARIABLE>': '<CLASS>' needs to have dll-interface to be used by clients of '<CLASS>'
    #pragma warning(disable:4355)   //  'this': used in base member initializer list
    #pragma warning(disable:4710)   //  '<FUNCTION>': function not inlined
    #pragma warning(disable:4711)   //  '<FUNCTION>': function selected for automatic inline expansion
    #pragma warning(disable:4866)   //  compiler may not enforce left-to-right evaluation order for call to 'C++17 operator'
    #pragma warning(disable:5045)   //  Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    #pragma warning(disable:5249)   //  '<CLASS>::<MEMBER>' of type '<TYPE' has named enumerators with values that cannot be represented in the given bit field width of '<WIDTH>'.
#else
    #error Unsupported C++ toolchain
#endif

//////////
//  emuone-util components
#include "emuone-util/Linkage.hpp"
#include "emuone-util/Classes.hpp"
#include "emuone-util/Macros.hpp"

//  Platform API
#include "emuone-util/Sync.hpp"
//  TODO ? #include "tt3-util/Locale.hpp"

//  Data types
//  TODO ? #include "tt3-util/DateTime.hpp"
#include "emuone-util/Exceptions.hpp"


//  Algorithms
#include "emuone-util/ToString.hpp"
#include "emuone-util/FromString.hpp"
//  TODO ? #include "tt3-util/Collections.hpp"

//  Stock objects
#include "emuone-util/StockObject.hpp"
//  TODO ? #include "tt3-util/MessageDigest.hpp"
//  TODO ? #include "tt3-util/License.hpp"
#include "emuone-util/Settings.hpp"
//  TODO ? #include "tt3-util/ResourceFactory.hpp"
//  TODO ? #include "tt3-util/Component.hpp"
//  TODO ? #include "tt3-util/Subsystem.hpp"
//  TODO ? #include "tt3-util/Tool.hpp"
#include "emuone-util/Dynaload.hpp"

//  Misc
//  TODO ? #include "tt3-util/ProductInformation.hpp"

//  End of emuone-util/API.hpp
