include(../emuone.pri)

TEMPLATE = lib
DEFINES += EMUONE_CORE_LIBRARY

SOURCES += \
    ApplianceTypeManager.cpp \
    ArchitectureManager.cpp \
    ComponentTypeManager.cpp \
    Dynaload.cpp \
    ITemplate.cpp \
    TemplateManager.cpp

HEADERS += \
    API.hpp \
    ApplianceType.hpp \
    Architecture.hpp \
    Classes.hpp \
    ComponentCategory.hpp \
    ComponentType.hpp \
    Dynaload.hpp \
    Linkage.hpp \
    Template.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -lemuone-util$$TARGET_SUFFIX
