include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    devices/ExternalFileSystem.cpp \
    devices/ExternalFileSystemEditor.cpp \
    kernel/Identity.cpp \
    kernel/Kernel.IdentityManager.cpp \
    kernel/Kernel.cpp \
    kernel/KernelEditor.cpp \
    kernel/Object.cpp \
    kernel/SystemIdentity.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp \
    devices/ExternalFileSystem.hpp \
    kernel/DataTypes.hpp \
    kernel/Identity.hpp \
    kernel/Kernel.hpp \
    kernel/Object.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-hades.qrc

FORMS += \
    devices/ExternalFileSystemEditor.ui \
    kernel/KernelEditor.ui
