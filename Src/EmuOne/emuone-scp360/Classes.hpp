//
//  emuone-scp360/Classes.hpp
//
//  Forward declarations & typedefs
//
//////////
#pragma once

namespace scp360
{
    class EMUONE_SCP360_EXPORT Object;
    class EMUONE_SCP360_EXPORT Segment;
    class EMUONE_SCP360_EXPORT SegmentMapping;
    class EMUONE_SCP360_EXPORT SegmentLock;
    class EMUONE_SCP360_EXPORT Process;
    class EMUONE_SCP360_EXPORT EnvironmentVariable;
    class EMUONE_SCP360_EXPORT IResource;

    class EMUONE_SCP360_EXPORT EmulatedApplication;
    class EMUONE_SCP360_EXPORT EmulatedProcess;

    class EMUONE_SCP360_EXPORT ObjectManager;
    class EMUONE_SCP360_EXPORT DeviceDriver;

    class EMUONE_SCP360_EXPORT SystemCall;
    class EMUONE_SCP360_EXPORT Scp;

    class EMUONE_SCP360_EXPORT InitProcess;

    using SegmentMappingList = QList<SegmentMapping*>;
    using SegmentLockList = QList<SegmentLock*>;

    using EmulatedApplicationList = QList<EmulatedApplication*>;
    using ProcessList = QList<Process*>;
}

//  End of emuone-scp360/Classes.hpp
