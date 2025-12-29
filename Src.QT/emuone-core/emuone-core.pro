include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_CORE_LIBRARY

SOURCES += \
    ArchitectureManager.cpp \
    Component.cpp \
    ComponentTypeManager.cpp \
    Exceptions.cpp \
    IComponent.cpp \
    IComponentType.cpp \
    IVirtualMachineTemplate.cpp \
    StandardComponentCategories.cpp \
    StandardVirtualMachineTypes.cpp \
    VirtualMachineTemplateManager.cpp \
    VirtualMachineTypeManager.cpp \
    VirtualMachine.cpp

HEADERS += \
    API.hpp \
    Architecture.hpp \
    Classes.hpp \
    Component.hpp \
    ComponentCategory.hpp \
    ComponentType.hpp \
    Components.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    VirtualMachine.hpp \
    VirtualMachineTemplate.hpp \
    VirtualMachineType.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -lemuone-util$$TARGET_SUFFIX

RESOURCES += \
    emuone-core.qrc
