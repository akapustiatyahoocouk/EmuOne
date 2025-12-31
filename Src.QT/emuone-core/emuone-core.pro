include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_CORE_LIBRARY

SOURCES += \
    ArchitectureManager.cpp \
    Component.cpp \
    ComponentAdaptorEditor.cpp \
    ComponentAdaptorTypeManager.cpp \
    ComponentCategoryManager.cpp \
    ComponentEditor.cpp \
    ComponentTypeManager.cpp \
    Exceptions.cpp \
    IComponent.cpp \
    IComponentAdaptorType.cpp \
    IComponentCategory.cpp \
    IComponentType.cpp \
    IDevice.cpp \
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
    ComponentAdaptor.hpp \
    ComponentAdaptorEditor.hpp \
    ComponentAdaptorType.hpp \
    ComponentCategory.hpp \
    ComponentEditor.hpp \
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
