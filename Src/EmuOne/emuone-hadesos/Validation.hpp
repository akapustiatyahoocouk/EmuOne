//
//  emuone-hadesos/Validation.hpp
//
//  The HADES OS validation helpers
//
//////////
#pragma once

namespace hadesos
{
    //////////
    //  The HADES OS validation helpers
    class EMUONE_HADESOS_EXPORT Validation final
    {
        UTILITY_CLASS(Validation)

        //////////
        //  Operations
    public:
        static bool         isValidVolumeName(const QString & volumeName);
    };
}

//  End of emuone-hadesos/Validation.hpp
