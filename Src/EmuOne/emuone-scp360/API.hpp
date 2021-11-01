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

#include "emuone-scp360/Classes.hpp"
#include "emuone-scp360/Types.hpp"
#include "emuone-scp360/Buffers.hpp"

#include "emuone-scp360/Objects.hpp"
#include "emuone-scp360/ObjectManager.hpp"
#include "emuone-scp360/Validator.hpp"

#include "emuone-scp360/DeviceDrivers.hpp"
#include "emuone-scp360/Resources.hpp"

#include "emuone-scp360/SystemCalls.hpp"

#include "emuone-scp360/EmulatedApplication.hpp"
#include "emuone-scp360/EmulatedProcess.hpp"
#include "emuone-scp360/InitProcess.hpp"
#include "emuone-scp360/LoginProcess.hpp"
#include "emuone-scp360/Scp.hpp"

//  End of emuone-scp360/API.hpp
