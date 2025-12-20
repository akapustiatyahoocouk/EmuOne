//
//  emuone-hades/Linkage.hpp - emuone-hades linkage definitions
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

#if defined(EMUONE_HADES_LIBRARY)
    #define EMUONE_HADES_PUBLIC     Q_DECL_EXPORT
#else
    #define EMUONE_HADES_PUBLIC     Q_DECL_IMPORT
#endif

//  End of emuone-hades/Linkage.hpp
