//
//  emuone-cereon/API.hpp
//
//  enuone-cereon master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"
#include "emuone-core/API.hpp"

//////////
//  emuone-cereon components
#if defined(EMUONECEREON_LIBRARY)
    #define EMUONE_CEREON_EXPORT  Q_DECL_EXPORT
#else
    #define EMUONE_CEREON_EXPORT  Q_DECL_IMPORT
#endif

#include "emuone-cereon/Classes.hpp"

#include "emuone-cereon/Architectures.hpp"
#include "emuone-cereon/Templates.hpp"

#include "emuone-cereon/Features.hpp"
#include "emuone-cereon/InstructionTypes.hpp"
#include "emuone-cereon/InstructionSets.hpp"

#include "emuone-cereon/Memory.hpp"
#include "emuone-cereon/RamUnitEditor.hpp"
#include "emuone-cereon/RomUnitEditor.hpp"
#include "emuone-cereon/Io.hpp"
#include "emuone-cereon/Processors.hpp"

#include "emuone-cereon/Cmos1.hpp"

//  End of emuone-cereon/API.hpp
