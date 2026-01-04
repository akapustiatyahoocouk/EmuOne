include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_HADES_LIBRARY

SOURCES += \
    Architecture.cpp \
    Component.cpp \
    devices/ExternalFileSystem.cpp \
    devices/ExternalFileSystemEditor.cpp \
    devices/TextTerminal.cpp \
    devices/TextTerminalEditor.cpp \
    kernel/Device.cpp \
    kernel/DeviceType.cpp \
    kernel/DeviceTypeId.cpp \
    kernel/Identity.cpp \
    kernel/Kernel.DeviceManagement.cpp \
    kernel/Kernel.DeviceTypeManagement.cpp \
    kernel/Kernel.IdentityManagement.cpp \
    kernel/Kernel.ObjectManagement.cpp \
    kernel/Kernel.cpp \
    kernel/KernelEditor.cpp \
    kernel/Object.cpp \
    kernel/Oid.cpp \
    kernel/Processor.cpp \
    kernel/ProcessorCore.cpp \
    kernel/SystemIdentity.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp \
    devices/ExternalFileSystem.hpp \
    devices/TextTerminal.hpp \
    kernel/DataTypes.hpp \
    kernel/Device.hpp \
    kernel/DeviceType.hpp \
    kernel/Executor.hpp \
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
    devices/TextTerminalEditor.ui \
    kernel/KernelEditor.ui

DISTFILES +=
