//
//  emuone-core/Classes.hpp - Forward declarations and typedefs
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::core
{
    //  Classes
    class IArchitecture;
    class IVirtualMachineType;
    class IVirtualMachineTemplate;
    class IComponentCategory;
    class IComponentType;
    class IComponent;
    class IComponentAdaptorType;
    class IComponentAdaptor;
    class VirtualMachine;

    class ComponentEditor;
    class ComponentAdaptorEditor;
    class DisplaySurface;

    //  Collections
    using Architectures = QSet<IArchitecture*>;
    using VirtualMachineTypes = QSet<IVirtualMachineType*>;
    using VirtualMachineTemplates = QSet<IVirtualMachineTemplate*>;
    using ComponentCategories = QSet<IComponentCategory*>;
    using ComponentTypes = QSet<IComponentType*>;
    using Components = QSet<IComponent*>;
    using ComponentAdaptorTypes = QSet<IComponentAdaptorType*>;
    using ComponentAdaptors = QSet<IComponentAdaptor*>;
    using VirtualMachines = QList<VirtualMachine*>;
    using DisplaySurfaces = QSet<DisplaySurface*>;
}

//  End of emuone-core/Classes.hpp
