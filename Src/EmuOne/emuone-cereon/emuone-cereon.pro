TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONECEREON_LIBRARY

SOURCES += \
    Architectures.cpp \
    InstructionSets.cpp \
    MemoryBus.cpp \
    Plugin.cpp \
    Processor.cpp \
    Processor1P1B.cpp \
    RamUnit.cpp \
    RamUnitEditor.cpp \
    RomUnit.cpp \
    RomUnitEditor.cpp \
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
    RamUnitEditor.hpp \
    RomUnitEditor.hpp \
    Templates.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

RESOURCES += \
    cereon.qrc

FORMS += \
    RamUnitEditor.ui \
    RomUnitEditor.ui
