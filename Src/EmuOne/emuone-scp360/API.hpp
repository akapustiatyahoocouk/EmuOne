//
//  emuone-scp360/API.hpp
//
//  enuone-scp360 master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"
#include "emuone-core/API.hpp"
#include "emuone-ibm360/API.hpp"

//////////
//  emuone-scp360 components
#if defined(EMUONESCP360_LIBRARY)
    #define EMUONE_SCP360_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_SCP360_EXPORT  Q_DECL_IMPORT
#endif


//  End of emuone-scp360/API.hpp