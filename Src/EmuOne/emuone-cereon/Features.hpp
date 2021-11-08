//
//  emuone-cereon/Features.hpp
//
//  Cereon Features
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  Cereon CPU features
    enum class Features : uint16_t
    {
        //  Individual features
        Base                    = 0x0001,
        FloatingPoint           = 0x0002,
        Debug                   = 0x0004,
        UnalignedOperands       = 0x0008,
        ProtectedMemory         = 0x0010,
        VirtualMemory           = 0x0020,
        PerformanceMonitoring   = 0x0040,
        //  Feature combinations
        None                    = 0x0000,
        All                     = 0x007F
    };
}

//  End of emuone-cereon/Features.hpp
