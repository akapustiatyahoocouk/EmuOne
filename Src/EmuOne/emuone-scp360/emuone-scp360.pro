QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONESCP360_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    DeviceDriver.cpp \
    EmulatedApplication.cpp \
    EmulatedProcess.cpp \
    EnvironmentVariable.cpp \
    Ibm2741Driver.cpp \
    InitProcess.cpp \
    Plugin.cpp \
    Process.cpp \
    Scp.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    DeviceDrivers.hpp \
    EmulatedApplication.hpp \
    InitProcess.hpp \
    Process.hpp \
    Scp.hpp

LIBS += -L../../../Bin/Debug -lemuone-ibm360 -lemuone-core -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
