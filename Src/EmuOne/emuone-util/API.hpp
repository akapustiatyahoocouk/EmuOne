//
//  emuone-util/API.hpp
//
//  enuone-util master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include <QtCore/qglobal.h>
#include <QCoreApplication>
#include <QDir>
#include <QException>
#include <QIcon>
#include <QLibrary>
#include <QSet>
#include <QSettings>

//////////
//  emuone-util components
#if defined(EMUONEUTIL_LIBRARY)
    #define EMUONE_UTIL_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_UTIL_EXPORT  Q_DECL_IMPORT
#endif

#include "emuone-util/Macros.hpp"
#include "emuone-util/PluginManager.hpp"

#include "emuone-util/StockObject.hpp"
//  End of emuone-util/API.hpp
