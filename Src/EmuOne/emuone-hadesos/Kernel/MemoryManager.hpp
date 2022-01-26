//
//  emuone-hadesos/Kernel/MemoryManager.hpp
//
//  The HADES OS "kernel memory manager" API
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  A generic "identity" that identifies a client
        class EMUONE_HADESOS_EXPORT IMemoryManager
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IMemoryManager)

            //////////
            //  This is an interface
        public:
            IMemoryManager() {}
            virtual ~IMemoryManager() {}
        };
    }
}

//  End of emuone-hadesos/Kernel/MemoryManager.hpp
