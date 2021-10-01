//
//  emuone-core/API.hpp
//
//  enuone-core master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"

//////////
//  emuone-core components
#if defined(EMUONECORE_LIBRARY)
    #define EMUONE_CORE_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_CORE_EXPORT  Q_DECL_IMPORT
#endif

#include "emuone-core/Classes.hpp"
#include "emuone-core/ComponentCategory.hpp"
#include "emuone-core/ComponentType.hpp"
//#include "emuone-core/Component.hpp"

//#include "emuone-core/VirtualAppliance.hpp"

//  End of emuone-core/API.hpp
