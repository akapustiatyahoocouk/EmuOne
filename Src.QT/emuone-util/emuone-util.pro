include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_UTIL_LIBRARY

SOURCES += \
    Dynaload.cpp \
    Exceptions.cpp \
    FromString.cpp \
    IComponent.cpp \
    Settings.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Dynaload.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    Settings.hpp \
    StockObject.hpp \
    ToString.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    emuone-util.qrc
