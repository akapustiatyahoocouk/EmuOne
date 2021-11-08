//
//  emuone-cereon/InstructionTypes.hpp
//
//  Cereon instruction types
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  A generic Cereon instruction type
    class EMUONE_CEREON_EXPORT InstructionType
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(InstructionType)

        //////////
        //  Construction/destruction - from derived singleton classes only
    protected:
        InstructionType(uint32_t mask, uint32_t bitPattern, Features features, bool privileged = false)
            :   _mask(mask), _bitPattern(bitPattern), _features(features), _privileged(privileged) {}
        virtual ~InstructionType() {}

        //////////
        //  Operations
    public:
        //  The mask to apply to asn instruction word before matching this instruction
        uint32_t            mask() const { return _mask; }

        //  The bit pattern of this i9nstruction after the "mask" has been applied
        uint32_t            bitPattern() const { return _bitPattern; }

        //  The feature(s) CPU must have for this instruction to be enabled
        Features            features() const { return _features; }

        //  True if this instruction is only allowed in Kernel mode
        bool                privileged() const { return _privileged; }

        //////////
        //  Implementation
    private:
        uint32_t            _mask;
        uint32_t            _bitPattern;
        Features            _features;
        bool                _privileged;
    };
}

//  End of emuone-cereon/InstructionTypes.hpp
