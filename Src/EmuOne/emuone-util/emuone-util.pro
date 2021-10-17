QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONEUTIL_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    CharacterSet.cpp \
    Cp037CharacterSet.cpp \
    Iso88591CharacterSet.cpp \
    Plugin.cpp \
    PluginManager.cpp

HEADERS += \
    API.hpp \
    BlockingQueue.hpp \
    Buffers.hpp \
    CharacterSets.hpp \
    Classes.hpp \
    Macros.hpp \
    PluginManager.hpp \
    StockObject.hpp

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    util.qrc
