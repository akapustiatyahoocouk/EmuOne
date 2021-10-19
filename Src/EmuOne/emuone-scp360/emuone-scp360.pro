QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONESCP360_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    Device.cpp \
    DeviceDriver.cpp \
    EmulatedApplication.cpp \
    EmulatedProcess.SystemCalls.cpp \
    EmulatedProcess.cpp \
    EnvironmentVariable.cpp \
    Ibm2741Driver.cpp \
    InitProcess.cpp \
    LoginProcess.cpp \
    Object.cpp \
    ObjectManager.cpp \
    PhysicalDevice.cpp \
    Plugin.cpp \
    Process.cpp \
    Resources.cpp \
    Scp.cpp \
    Segment.cpp \
    SegmentLock.cpp \
    SegmentMapping.cpp \
    SystemCalls.cpp \
    Types.cpp

HEADERS += \
    API.hpp \
    Buffers.hpp \
    Classes.hpp \
    DeviceDrivers.hpp \
    EmulatedApplication.hpp \
    EmulatedProcess.hpp \
    InitProcess.hpp \
    LoginProcess.hpp \
    ObjectManager.hpp \
    Objects.hpp \
    Resources.hpp \
    Scp.hpp \
    SystemCalls.hpp \
    Types.hpp

LIBS += -L../../../Bin/Debug -lemuone-ibm360 -lemuone-core -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
