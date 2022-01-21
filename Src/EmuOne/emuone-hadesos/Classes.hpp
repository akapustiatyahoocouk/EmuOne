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
        class EMUONE_HADESOS_EXPORT Object;

        class EMUONE_HADESOS_EXPORT Identity;
        class EMUONE_HADESOS_EXPORT UserIdentity;
        class EMUONE_HADESOS_EXPORT SyntheticIdentity;
        class EMUONE_HADESOS_EXPORT GroupIdentity;

        class EMUONE_HADESOS_EXPORT Atom;
        class EMUONE_HADESOS_EXPORT AtomLock;

        class EMUONE_HADESOS_EXPORT Process;

        class EMUONE_HADESOS_EXPORT Kernel;

        using IdentityList = QList<Identity*>;
        using GroupIdentityList = QList<GroupIdentity*>;
        using AtomLockList = QList<AtomLock*>;
    }
}

namespace hadesos
{
    class EMUONE_HADESOS_EXPORT HadesOsEditor;
    using HadesOsEditorList = QList<HadesOsEditor*>;
}
//  End of emuone-hadesos/Classes.hpp
