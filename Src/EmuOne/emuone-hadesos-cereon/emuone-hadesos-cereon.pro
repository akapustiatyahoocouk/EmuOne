TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEHADESOSCEREON_LIBRARY

SOURCES += \
    Adaptor.Type.cpp \
    Adaptor.cpp \
    Plugin.cpp

HEADERS += \
    API.hpp \
    Adaptor.hpp \
    MemoryManager.hpp

LIBS += -L../../../Bin/Debug -lemuone-hadesos -lemuone-cereon -lemuone-core -lemuone-util

RESOURCES +=

FORMS +=
