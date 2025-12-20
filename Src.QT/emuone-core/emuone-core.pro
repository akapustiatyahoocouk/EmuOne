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
    StereotypeManager.cpp \
    TemplateManager.cpp

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
    Stereotype.hpp \
    Template.hpp \
    VirtualMachine.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -lemuone-util$$TARGET_SUFFIX
