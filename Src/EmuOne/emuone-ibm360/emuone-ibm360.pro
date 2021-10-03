QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONEIBM360_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Ibm360Architecture.cpp \
    Plugin.cpp \
    VirtualMachineTemplates.cpp

HEADERS += \
    API.hpp \
    API.hpp \
    Architecture.hpp \
    Templates.hpp

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ibm360.qrc
