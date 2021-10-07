QT += core gui widgets xml

TEMPLATE = lib
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += EMUONEIBM360_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

SOURCES += \
    Architecture.cpp \
    Device.cpp \
    Ibm2741.cpp \
    Ibm2741Editor.cpp \
    Ibm2741FullScreenWidget.cpp \
    Ibm360StorageEditor.cpp \
    Plugin.cpp \
    Storage.cpp \
    Templates.cpp

HEADERS += \
    API.hpp \
    API.hpp \
    Architecture.hpp \
    Device.hpp \
    Features.hpp \
    Ibm2741.hpp \
    Ibm2741Editor.hpp \
    Ibm2741FullScreenWidget.hpp \
    Ibm360StorageEditor.hpp \
    Storage.hpp \
    Templates.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ibm360.qrc

FORMS += \
    Ibm2741Editor.ui \
    Ibm2741FullScreenWidget.ui \
    Ibm360StorageEditor.ui
