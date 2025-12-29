include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Component.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-hades.qrc
