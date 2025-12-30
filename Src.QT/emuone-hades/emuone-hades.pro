include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    devices/ExternalFileSystem.cpp \
    devices/ExternalFileSystemEditor.cpp \
    kernel/EditSharedFolderDialog.cpp \
    kernel/Kernel.cpp \
    kernel/KernelEditor.cpp \
    kernel/SharedFolder.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp \
    devices/ExternalFileSystem.hpp \
    kernel/EditSharedFolderDialog.hpp \
    kernel/Kernel.hpp

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-hades.qrc

FORMS += \
    devices/ExternalFileSystemEditor.ui \
    kernel/EditSharedFolderDialog.ui \
    kernel/KernelEditor.ui
