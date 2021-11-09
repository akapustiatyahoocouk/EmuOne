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

    inline Features operator | (Features op1, Features op2)
    {
        return Features(uint16_t(op1) | uint16_t(op2));
    }

    inline Features operator & (Features op1, Features op2)
    {
        return Features(uint16_t(op1) & uint16_t(op2));
    }

    inline Features & operator |= (Features & op1, Features op2)
    {
        op1 = Features(uint16_t(op1) | uint16_t(op2));
        return op1;
    }

    inline Features & operator &= (Features & op1, Features op2)
    {
        op1 = Features(uint16_t(op1) & uint16_t(op2));
        return op1;
    }
}

//  End of emuone-cereon/Features.hpp
