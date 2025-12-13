//
//  emuone-core/Classes.cpp - Forward declarations and typedefs
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
    class EMUONE_CORE_PUBLIC IArchitecture;
    class EMUONE_CORE_PUBLIC IApplianceType;
    class EMUONE_CORE_PUBLIC ITemplate;
    class EMUONE_CORE_PUBLIC IComponentCategory;
    class EMUONE_CORE_PUBLIC IComponentType;
    class EMUONE_CORE_PUBLIC IComponent;

    //  Collections
    using Architectures = QSet<IArchitecture*>;
    using ApplianceTypes = QSet<IApplianceType*>;
    using Templates = QSet<ITemplate*>;
    using ComponentCategories = QSet<IComponentCategory*>;
    using ComponentTypes = QSet<IComponentType*>;
    using Components = QSet<IComponent*>;
}

//  End of emuone-core/Classes.cpp
