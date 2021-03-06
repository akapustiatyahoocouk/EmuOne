TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONECEREON_LIBRARY

SOURCES += \
    Architectures.cpp \
    Cmos1.cpp \
    Cmos1Editor.cpp \
    IoBus.cpp \
    MemoryBus.cpp \
    Plugin.cpp \
    Processor.cpp \
    Processor1P1B.cpp \
    Processor1P1BEditor.cpp \
    ProcessorCore.cpp \
    RamUnit.cpp \
    RamUnitEditor.cpp \
    RomUnit.cpp \
    RomUnitEditor.cpp \
    Templates.cpp

HEADERS += \
    API.hpp \
    Architectures.hpp \
    Classes.hpp \
    Cmos1.hpp \
    Cmos1Editor.hpp \
    Io.hpp \
    Memory.hpp \
    Monitor.hpp \
    Processor1P1BEditor.hpp \
    Processors.hpp \
    RamUnitEditor.hpp \
    RomUnitEditor.hpp \
    Templates.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

RESOURCES += \
    cereon.qrc

FORMS += \
    Cmos1Editor.ui \
    Processor1P1BEditor.ui \
    RamUnitEditor.ui \
    RomUnitEditor.ui
