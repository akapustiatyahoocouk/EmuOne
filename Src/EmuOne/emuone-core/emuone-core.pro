TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONECORE_LIBRARY

SOURCES += \
    Adaptor.cpp \
    Architecture.cpp \
    ClockFrequency.cpp \
    Component.cpp \
    ComponentCategory.cpp \
    ComponentEditor.cpp \
    ComponentType.cpp \
    Duration.cpp \
    FullScreenWidget.cpp \
    MemorySize.cpp \
    Plugin.cpp \
    RemoteTerminal.cpp \
    StandardComponentCategories.cpp \
    VirtualAppliance.cpp \
    VirtualApplianceType.cpp \
    VirtualMachine.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    ClockFrequency.hpp \
    Component.hpp \
    ComponentAspect.hpp \
    ComponentCategory.hpp \
    ComponentEditor.hpp \
    ComponentType.hpp \
    ComponentUi.hpp \
    Duration.hpp \
    Exceptions.hpp \
    FullScreenWidget.hpp \
    MemorySize.hpp \
    Template.hpp \
    VirtualAppliance.hpp

LIBS += -L../../../Bin/Debug -lemuone-util

RESOURCES += \
    core.qrc

FORMS +=
