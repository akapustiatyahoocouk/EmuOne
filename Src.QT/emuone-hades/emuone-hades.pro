include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    Kernel.cpp \
    KernelEditor.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    Kernel.hpp \
    Linkage.hpp

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-hades.qrc

FORMS += \
    KernelEditor.ui
