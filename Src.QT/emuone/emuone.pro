include(../emuone.pri)

SOURCES += \
    Application.cpp \
    Component.cpp \
    ConfigureVirtualMachineDialog.cpp \
    KnownVirtualMachine.cpp \
    Main.cpp \
    MainFrame.cpp \
    NewVirtualMachineDialog.cpp \
    RunningVirtualMachineView.cpp \
    StoppedVirtualMachineView.cpp \
    VirtualMachinePage.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Component.hpp \
    ConfigureVirtualMachineDialog.hpp \
    KnownVirtualMachine.hpp \
    MainFrame.hpp \
    NewVirtualMachineDialog.hpp \
    RunningVirtualMachineView.hpp \
    StoppedVirtualMachineView.hpp \
    VirtualMachinePage.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \
    ConfigureVirtualMachineDialog.ui \
    MainFrame.ui \
    NewVirtualMachineDialog.ui \
    RunningVirtualMachineView.ui \
    StoppedVirtualMachineView.ui \
    VirtualMachinePage.ui

RESOURCES += \
    emuone.qrc

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX
