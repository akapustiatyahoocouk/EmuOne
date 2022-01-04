TEMPLATE = lib
include(../emuone.pri)

DEFINES += EMUONECORE_LIBRARY

SOURCES += \
    Adaptor.cpp \
    AdaptorEditor.cpp \
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
    Adaptor.hpp \
    AdaptorEditor.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    ComponentEditor.hpp \
    ComponentUi.hpp \
    DataTypes.hpp \
    Exceptions.hpp \
    FullScreenWidget.hpp \
    Template.hpp \
    VirtualAppliance.hpp

LIBS += -L../../../Bin/Debug -lemuone-util

RESOURCES += \
    core.qrc

FORMS +=
