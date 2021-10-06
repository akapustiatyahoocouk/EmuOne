QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONECORE_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    ComponentCategory.cpp \
    ComponentEditor.cpp \
    ComponentType.cpp \
    MemorySize.cpp \
    Plugin.cpp \
    RemoteTerminal.cpp \
    StandardComponentCategories.cpp \
    VirtualAppliance.cpp \
    VirtualApplianceType.cpp \
    VirtualMachine.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    ComponentCategory.hpp \
    ComponentEditor.hpp \
    ComponentType.hpp \
    Exceptions.hpp \
    MemorySize.hpp \
    Template.hpp \
    VirtualAppliance.hpp

LIBS += -L../../../Bin/Debug -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    core.qrc
