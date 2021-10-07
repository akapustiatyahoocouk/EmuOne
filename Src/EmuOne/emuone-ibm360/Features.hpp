//
//  emuone-ibm360/Features.hpp
//
//  IBM/360 optional features
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  IBM/360 optional features
    enum class Features : uint8_t
    {
        FetchProtection     = 0x01, //  Implies StoreProtection
        StoreProtection     = 0x02,
        Decimal             = 0x04,
        FloatingPoint       = 0x08,
        ByteOrientedOperand = 0x10,
        Timer               = 0x20,
        DirectControl       = 0x40
    };

    //////////
    //  Helper functions
    EMUONE_IBM360_EXPORT Features operator & (Features op1, Features op2);
    EMUONE_IBM360_EXPORT Features operator | (Features op1, Features op2);
    EMUONE_IBM360_EXPORT Features & operator &= (Features & op1, Features op2);
    EMUONE_IBM360_EXPORT Features & operator |= (Features & op1, Features op2);

    EMUONE_IBM360_EXPORT Features normalise(Features features);
}

//  End of emuone-ibm360/Features.hpp
