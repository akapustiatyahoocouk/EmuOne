TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEHADESOS_LIBRARY

SOURCES += \
    HadesOs.Type.cpp \
    HadesOs.cpp \
    Plugin.cpp

HEADERS += \
    API.hpp \
    HadesOs.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

RESOURCES +=

FORMS +=
