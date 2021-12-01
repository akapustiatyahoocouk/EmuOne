//
//  emuone-util/API.hpp
//
//  enuone-util master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-promo"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <QtCore/qglobal.h>
#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDir>
#include <QDomDocument>
#include <QException>
#include <QFileDialog>
#include <QGridLayout>
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

#pragma GCC diagnostic pop

//////////
//  emuone-util components
#if defined(EMUONEUTIL_LIBRARY)
    #define EMUONE_UTIL_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_UTIL_EXPORT  Q_DECL_IMPORT
#endif

#include "emuone-util/Macros.hpp"
#include "emuone-util/PluginManager.hpp"

#include "emuone-util/Memory.hpp"
#include "emuone-util/TimeSpan.hpp"
#include "emuone-util/TimeStamp.hpp"

#include "emuone-util/StockObject.hpp"

#include "emuone-util/CharacterSets.hpp"
#include "emuone-util/BlockingQueue.hpp"
#include "emuone-util/Buffers.hpp"

#include "emuone-util/MessageDigest.hpp"

//  End of emuone-util/API.hpp
