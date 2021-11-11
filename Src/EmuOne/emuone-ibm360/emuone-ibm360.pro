TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONEIBM360_LIBRARY

SOURCES += \
    Architecture.cpp \
    Device.cpp \
    Features.cpp \
    Ibm2741.cpp \
    Ibm2741Editor.cpp \
    Ibm2741FullScreenWidget.cpp \
    Ibm360ProcessorEditor.cpp \
    Ibm360StorageEditor.cpp \
    Plugin.cpp \
    Processor.cpp \
    Storage.cpp \
    Templates.cpp

HEADERS += \
    API.hpp \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Device.hpp \
    Features.hpp \
    Ibm2741.hpp \
    Ibm2741Editor.hpp \
    Ibm2741FullScreenWidget.hpp \
    Ibm360ProcessorEditor.hpp \
    Ibm360StorageEditor.hpp \
    Monitor.hpp \
    Processor.hpp \
    Storage.hpp \
    Templates.hpp

LIBS += -L../../../Bin/Debug -lemuone-core -lemuone-util

RESOURCES += \
    Ibm360.qrc

FORMS += \
    Ibm2741Editor.ui \
    Ibm2741FullScreenWidget.ui \
    Ibm360ProcessorEditor.ui \
    Ibm360StorageEditor.ui
