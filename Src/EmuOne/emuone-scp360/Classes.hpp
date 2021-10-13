//
//  emuone-scp360/Classes.hpp
//
//  Forward declarations & typedefs
//
//////////
#pragma once

namespace scp360
{
    class EMUONE_SCP360_EXPORT EmulatedApplication;
    class EMUONE_SCP360_EXPORT Process;
    class EMUONE_SCP360_EXPORT EmulatedProcess;
    class EMUONE_SCP360_EXPORT Scp;

    using EmulatedApplicationList = QList<EmulatedApplication*>;
    using ProcessList = QList<Process*>;
}

//  End of emuone-scp360/Classes.hpp
