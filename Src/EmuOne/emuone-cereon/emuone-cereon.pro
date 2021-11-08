QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONECEREON_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    Architectures.cpp \
    InstructionSets.cpp \
    Plugin.cpp \
    Processor.cpp \
    Processor1P1B.cpp \
    ResidentRamUnit.cpp \
    ResidentRomUnit.cpp \
    Templates.cpp

HEADERS += \
    API.hpp \
    Architectures.hpp \
    Classes.hpp \
    Features.hpp \
    InstructionSets.hpp \
    InstructionTypes.hpp \
    Memory.hpp \
    Processors.hpp \
    Templates.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cereon.qrc
