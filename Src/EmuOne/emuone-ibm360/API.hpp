//
//  emuone-ibm360/API.hpp
//
//  enuone-ibm360 master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"
#include "emuone-core/API.hpp"

//////////
//  emuone-ibm360 components
#if defined(EMUONEIBM360_LIBRARY)
    #define EMUONE_IBM360_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_IBM360_EXPORT  Q_DECL_IMPORT
#endif

#include "emuone-ibm360/Architecture.hpp"
#include "emuone-ibm360/Templates.hpp"

#include "emuone-ibm360/Storage.hpp"

//  End of emuone-ibm360/API.hpp
