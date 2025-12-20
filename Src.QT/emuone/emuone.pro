include(../emuone.pri)

SOURCES += \
    Application.cpp \
    Dynaload.cpp \
    Main1.cpp \
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

LIBS += \
    -lemuone-core$$TARGET_SUFFIX    \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone.qrc
