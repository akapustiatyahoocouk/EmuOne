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
        DirectControl       = 0x40,
        //  Flag combinations
        None                = 0x00,
        All                 = 0x7F,
        Default             = StoreProtection | Timer
    };

    //////////
    //  Helper functions
    inline EMUONE_IBM360_EXPORT Features operator & (Features op1, Features op2) { return Features(uint8_t(op1) & uint8_t(op2)); }
    inline EMUONE_IBM360_EXPORT Features operator | (Features op1, Features op2) { return Features(uint8_t(op1) | uint8_t(op2)); }
    inline EMUONE_IBM360_EXPORT Features operator ~ (Features op) { return Features(~uint8_t(op) & uint8_t(Features::All)); }

    inline EMUONE_IBM360_EXPORT Features & operator &= (Features & op1, Features op2) { op1 = op1 & op2; return op1; }
    inline EMUONE_IBM360_EXPORT Features & operator |= (Features & op1, Features op2) { op1 = op1 | op2; return op1; }

    EMUONE_IBM360_EXPORT Features normalise(Features features);
    inline EMUONE_IBM360_EXPORT bool has(Features features, Features requiredFeatures) { return (uint8_t(features) & uint8_t(requiredFeatures)) == uint8_t(requiredFeatures); }
}

//  End of emuone-ibm360/Features.hpp
