TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEUTIL_LIBRARY

SOURCES += \
    CharacterSet.cpp \
    Cp037CharacterSet.cpp \
    Iso88591CharacterSet.cpp \
    MessageDigest.cpp \
    Plugin.cpp \
    PluginManager.cpp \
    Sha1MessageDigest.cpp \
    TimeSpan.cpp

HEADERS += \
    API.hpp \
    BlockingQueue.hpp \
    Buffers.hpp \
    CharacterSets.hpp \
    Classes.hpp \
    Macros.hpp \
    Memory.hpp \
    MessageDigest.hpp \
    PluginManager.hpp \
    StockObject.hpp \
    TimeSpan.hpp

RESOURCES += \
    util.qrc
