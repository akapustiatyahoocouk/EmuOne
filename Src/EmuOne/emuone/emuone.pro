include(../emuone.pri)

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConfigureVmDialog.cpp \
    Main.cpp \
    NewVmDialog.cpp \
    VirtualApplianceWindow.cpp \
    MainWindow.cpp

HEADERS += \
    API.hpp \
    ConfigureVmDialog.hpp \
    MainWindow.hpp \
    NewVmDialog.hpp \
    VirtualApplianceWindow.hpp

FORMS += \
    ConfigureVmDialog.ui \
    MainWindow.ui \
    NewVmDialog.ui \
    VirtualApplianceWindow.ui

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    emuone.qrc
