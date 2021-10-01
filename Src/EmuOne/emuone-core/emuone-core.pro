QT += gui

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONECORE_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    Plugin.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    ComponentCategory.hpp \
    ComponentType.hpp \
    Exceptions.hpp \
    Template.hpp \
    VirtualAppliance.hpp

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
