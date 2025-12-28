include(../emuone.pri)

SOURCES += \
    Application.cpp \
    Dynaload.cpp \
    Main.cpp \
    MainFrame.cpp \
    NewVirtualMachineDialog.cpp \
    StoppedVirtualMachineView.cpp \
    VirtualMachinePage.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Dynaload.hpp \
    MainFrame.hpp \
    NewVirtualMachineDialog.hpp \
    StoppedVirtualMachineView.hpp \
    VirtualMachinePage.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \
    MainFrame.ui \
    NewVirtualMachineDialog.ui \
    StoppedVirtualMachineView.ui \
    VirtualMachinePage.ui

RESOURCES += \
    emuone.qrc

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX
