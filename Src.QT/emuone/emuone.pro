include(../emuone.pri)

SOURCES += \
    Application.cpp \
    Dynaload.cpp \
    Main.cpp \
    MainFrame.cpp \
    NewVirtualMachineDialog.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Dynaload.hpp \
    MainFrame.hpp \
    NewVirtualMachineDialog.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \
    MainFrame.ui \
    NewVirtualMachineDialog.ui

RESOURCES += \
    emuone.qrc

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX
