//
//  emuone-hadesos-cereon/API.hpp
//
//  enuone-hadesos-cereon master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"
#include "emuone-core/API.hpp"
#include "emuone-hadesos/API.hpp"
#include "emuone-cereon/API.hpp"

//////////
//  emuone-hadesos-cereon components
#if defined(EMUONEHADESOSCEREON_LIBRARY)
    #define EMUONE_HADESOS_CEREON_EXPORT    Q_DECL_EXPORT
#else
    #define EMUONE_HADESOS_CEREON_EXPORT    Q_DECL_IMPORT
#endif

//#include "emuone-hadesos-cereon/Classes.hpp"

#include "emuone-hadesos-cereon/Adaptor.hpp"

//  End of emuone-hadesos-cereon/API.hpp
