//
//  emuone-hadesos/Kernel.hpp
//
//  The HADES OS Kernel
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  A HADES OS Kernel "resulot code" - any kernel service will return one
        enum ErrorCode : uint32_t
        {
            Success = 0x00000000,

            //  Misc
            Unknown = 0xFFFFFFFF
        };

        //////////
        //  A HADES OS Kernel
        class EMUONE_HADESOS_EXPORT Kernel final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Kernel)

            //////////
            //  Object tables
        private:
            QMap<Atom::Id, Atom*>   _atoms; //  allAtoms that are currently live
            QQueue<Atom::Id>        _recycledAtomIds;   //  IDs of destroyed atoms, ready for quick recycling
        };
    }
}

//  End of emuone-hadesos/Kernel.hpp
