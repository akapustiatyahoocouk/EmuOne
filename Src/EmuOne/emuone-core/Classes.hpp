//
//  emuone-core/Classes.hpp
//
//  Forward declarations and typedefs
//
//////////
#pragma once

class EMUONE_CORE_EXPORT Architecture;
class EMUONE_CORE_EXPORT VirtualApplianceTemplate;
class EMUONE_CORE_EXPORT VirtualMachineTemplate;
class EMUONE_CORE_EXPORT RemoteTerminalTemplate;
class EMUONE_CORE_EXPORT ComponentCategory;
class EMUONE_CORE_EXPORT ComponentType;
class EMUONE_CORE_EXPORT VirtualAppliance;

using ArchitectureList = QList<Architecture*>;
using VirtualApplianceTemplateList = QList<VirtualApplianceTemplate*>;
using VirtualMachineTemplateList = QList<VirtualMachineTemplate*>;
using RemoteTerminalTemplateList = QList<RemoteTerminalTemplate*>;
using ComponentCategoryList = QList<ComponentCategory*>;
using ComponentTypeList = QList<ComponentType*>;
using VirtualApplianceList = QList<VirtualAppliance*>;

//  End of emuone-core/Classes.hpp
