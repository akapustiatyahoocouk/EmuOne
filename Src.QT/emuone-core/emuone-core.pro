include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_CORE_LIBRARY

SOURCES += \
    ArchitectureManager.cpp \
    ComponentTypeManager.cpp \
    Dynaload.cpp \
    Exceptions.cpp \
    IComponent.cpp \
    IComponentType.cpp \
    ITemplate.cpp \
    StandardVirtualMachineTypes.cpp \
    VirtualMachineTypeManager.cpp \
    TemplateManager.cpp \
    VirtualMachine.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    ComponentCategory.hpp \
    ComponentType.hpp \
    Dynaload.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Template.hpp \
    VirtualMachine.hpp \
    VirtualMachineType.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-core.qrc
