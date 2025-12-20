include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Dynaload.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Dynaload.hpp \
    Linkage.hpp

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-hades.qrc
