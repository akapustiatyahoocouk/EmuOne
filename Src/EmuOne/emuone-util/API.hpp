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
#include <QDomDocument>
#include <QException>
#include <QFileDialog>
#include <QIcon>
#include <QLibrary>
#include <QMenu>
#include <QMessageBox>
#include <QRecursiveMutex>
#include <QSet>
#include <QSettings>
#include <QWidget>

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
