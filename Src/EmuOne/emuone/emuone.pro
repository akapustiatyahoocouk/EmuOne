QT       += core gui widgets

CONFIG += c++17

INCLUDEPATH += ..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    NewVmDialog.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    API.hpp \
    MainWindow.hpp \
    NewVmDialog.hpp

FORMS += \
    MainWindow.ui \
    NewVmDialog.ui

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    emuone.qrc
