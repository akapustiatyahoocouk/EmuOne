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
#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDir>
#include <QDomDocument>
#include <QException>
#include <QFileDialog>
#include <QIcon>
#include <QLibrary>
#include <QMenu>
#include <QMessageBox>
#include <QMutexLocker>
#include <QPainter>
#include <QQueue>
#include <QRecursiveMutex>
#include <QScreen>
#include <QSemaphore>
#include <QSet>
#include <QSettings>
#include <QTimer>
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

#include "emuone-util/CharacterSets.hpp"
#include "emuone-util/BlockingQueue.hpp"

//  End of emuone-util/API.hpp
