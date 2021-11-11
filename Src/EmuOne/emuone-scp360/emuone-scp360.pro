TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONESCP360_LIBRARY

SOURCES += \
    Device.cpp \
    DeviceDriver.cpp \
    EmulatedApplication.cpp \
    EmulatedProcess.SystemCalls.cpp \
    EmulatedProcess.cpp \
    EnvironmentVariable.cpp \
    Ibm2741Driver.cpp \
    InitProcess.cpp \
    LoginProcess.cpp \
    ModifySharedFolderDialog.cpp \
    NewSharedFolderDialog.cpp \
    Object.cpp \
    ObjectManager.cpp \
    PhysicalDevice.cpp \
    Plugin.cpp \
    Process.cpp \
    Resources.cpp \
    Scp.cpp \
    ScpEditor.cpp \
    Segment.cpp \
    SegmentLock.cpp \
    SegmentMapping.cpp \
    SystemCalls.cpp \
    Types.cpp \
    Validator.cpp

HEADERS += \
    API.hpp \
    Buffers.hpp \
    Classes.hpp \
    DeviceDrivers.hpp \
    EmulatedApplication.hpp \
    EmulatedProcess.hpp \
    InitProcess.hpp \
    LoginProcess.hpp \
    ModifySharedFolderDialog.hpp \
    NewSharedFolderDialog.hpp \
    ObjectManager.hpp \
    Objects.hpp \
    Resources.hpp \
    Scp.hpp \
    ScpEditor.hpp \
    SystemCalls.hpp \
    Types.hpp \
    Validator.hpp

LIBS += -L../../../Bin/Debug -lemuone-ibm360 -lemuone-core -lemuone-util

FORMS += \
    ModifySharedFolderDialog.ui \
    NewSharedFolderDialog.ui \
    ScpEditor.ui

RESOURCES += \
    scp360.qrc
