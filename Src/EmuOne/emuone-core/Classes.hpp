//
//  emuone-core/Classes.hpp
//
//  Forward declarations and typedefs
//
//////////
#pragma once

namespace core
{
    class EMUONE_CORE_EXPORT Architecture;
    class EMUONE_CORE_EXPORT VirtualApplianceTemplate;
    class EMUONE_CORE_EXPORT VirtualMachineTemplate;
    class EMUONE_CORE_EXPORT RemoteTerminalTemplate;
    class EMUONE_CORE_EXPORT ComponentCategory;
    class EMUONE_CORE_EXPORT ComponentType;
    class EMUONE_CORE_EXPORT Component;
    class EMUONE_CORE_EXPORT Adaptor;
    class EMUONE_CORE_EXPORT VirtualAppliance;
    class EMUONE_CORE_EXPORT VirtualMachine;
    class EMUONE_CORE_EXPORT RemoteTerminal;
    class EMUONE_CORE_EXPORT ComponentUi;

    using ArchitectureList = QList<Architecture*>;
    using VirtualApplianceTemplateList = QList<VirtualApplianceTemplate*>;
    using VirtualMachineTemplateList = QList<VirtualMachineTemplate*>;
    using RemoteTerminalTemplateList = QList<RemoteTerminalTemplate*>;
    using ComponentCategoryList = QList<ComponentCategory*>;
    using ComponentTypeList = QList<ComponentType*>;
    using ComponentList = QList<Component*>;
    using AdaptorList = QList<Adaptor*>;
    using VirtualApplianceList = QList<VirtualAppliance*>;
}

//  UI classes cannot be namespaced in QT

namespace core
{
    class EMUONE_CORE_EXPORT ComponentEditor;
    class EMUONE_CORE_EXPORT FullScreenWidget;

    using FullScreenWidgetList = QList<FullScreenWidget*>;
}

//  End of emuone-core/Classes.hpp
