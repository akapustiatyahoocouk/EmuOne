TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEHADESOS_LIBRARY

SOURCES += \
    AddSharedHostFolderDialog.cpp \
    HadesOs.Type.cpp \
    HadesOs.cpp \
    HadesOsEditor.cpp \
    Plugin.cpp \
    Validation.cpp

HEADERS += \
    API.hpp \
    AddSharedHostFolderDialog.hpp \
    HadesOs.hpp \
    HadesOsEditor.hpp \
    Validation.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

RESOURCES += \
    hadesos.qrc

FORMS += \
    AddSharedHostFolderDialog.ui \
    HadesOsEditor.ui
