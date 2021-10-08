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

#include "emuone-ibm360/Classes.hpp"

#include "emuone-ibm360/Architecture.hpp"
#include "emuone-ibm360/Templates.hpp"

#include "emuone-ibm360/Features.hpp"
#include "emuone-ibm360/Storage.hpp"
#include "emuone-ibm360/Ibm360StorageEditor.hpp"
#include "emuone-ibm360/Processor.hpp"
#include "emuone-ibm360/Ibm360ProcessorEditor.hpp"

#include "emuone-ibm360/Device.hpp"

#include "emuone-ibm360/Ibm2741.hpp"
#include "emuone-ibm360/Ibm2741Editor.hpp"
#include "emuone-ibm360/Ibm2741FullScreenWidget.hpp"

//  End of emuone-ibm360/API.hpp
