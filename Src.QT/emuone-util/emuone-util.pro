include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentManager.cpp \
    Exceptions.cpp \
    FromString.cpp \
    IComponent.cpp \
    Mutex.cpp \
    Settings.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    Memory.hpp \
    Settings.hpp \
    StockObject.hpp \
    Sync.hpp \
    ToString.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    emuone-util.qrc
