//
//  emuone-hadesos/API.hpp
//
//  enuone-hadesos master header
//
//////////
#pragma once

//////////
//  CRT + dependencies
#include "emuone-util/API.hpp"
#include "emuone-core/API.hpp"

//////////
//  emuone-hadesos components
#if defined(EMUONEHADESOS_LIBRARY)
    #define EMUONE_HADESOS_EXPORT   Q_DECL_EXPORT
#else
    #define EMUONE_HADESOS_EXPORT   Q_DECL_IMPORT
#endif

#include "emuone-hadesos/Classes.hpp"

#include "emuone-hadesos/Validation.hpp"
#include "emuone-hadesos/HadesOs.hpp"
#include "emuone-hadesos/HadesOsEditor.hpp"
#include "emuone-hadesos/AddSharedHostFolderDialog.hpp"
#include "emuone-hadesos/ModifySharedHostFolderDialog.hpp"

#include "emuone-hadesos/Kernel/Object.hpp"
#include "emuone-hadesos/Kernel/Identity.hpp"
#include "emuone-hadesos/Kernel/Atom.hpp"
#include "emuone-hadesos/Kernel/Process.hpp"

#include "emuone-hadesos/Kernel/MemoryManager.hpp"

#include "emuone-hadesos/Kernel/Kernel.hpp"

//  End of emuone-hadesos/API.hpp
