TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEUTIL_LIBRARY

SOURCES += \
    CharacterSet.cpp \
    Cp037CharacterSet.cpp \
    FromString.cpp \
    Iso88591CharacterSet.cpp \
    Memory.cpp \
    MessageDigest.cpp \
    Mutex.cpp \
    Plugin.cpp \
    PluginManager.cpp \
    Sha1MessageDigest.cpp \
    TimeSpan.cpp \
    TimeStamp.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    BlockingQueue.hpp \
    Buffers.hpp \
    CharacterSets.hpp \
    Classes.hpp \
    FromString.hpp \
    Macros.hpp \
    Memory.hpp \
    MessageDigest.hpp \
    PluginManager.hpp \
    StockObject.hpp \
    Sync.hpp \
    TimeSpan.hpp \
    TimeStamp.hpp \
    ToString.hpp

RESOURCES += \
    util.qrc
