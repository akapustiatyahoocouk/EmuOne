//
//  emuone-hadesos/Classes.hpp
//
//  Forward declarations and typedefs
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        class EMUONE_HADESOS_EXPORT Atom;
        class EMUONE_HADESOS_EXPORT AtomLock;
        class EMUONE_HADESOS_EXPORT Process;

        using AtomLockList = QList<AtomLock*>;
    }
}

//  End of emuone-hadesos/Classes.hpp
