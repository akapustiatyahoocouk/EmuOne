include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_UTIL_LIBRARY

SOURCES += \
    ComponentManager.cpp \
    Dynaload.cpp \
    Exceptions.cpp \
    FromString.cpp \
    IComponent.cpp \
    Mutex.cpp \
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
    Sync.hpp \
    ToString.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    emuone-util.qrc
