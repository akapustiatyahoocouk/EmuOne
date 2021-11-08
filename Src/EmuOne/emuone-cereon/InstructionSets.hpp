//
//  emuone-cereon/InstructionSets.hpp
//
//  Cereon instruction sets
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  The bit representation of an instruction
    typedef uint32_t Instruction;

    //////////
    //  A generic Cereon instruction set
    class EMUONE_CEREON_EXPORT InstructionSet
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(InstructionSet)

        //////////
        //  Construction/destruction - from derived singleton classes only
    protected:
        InstructionSet() {}
        virtual ~InstructionSet() {}

        //////////
        //  Operations
    public:
        //  The ISA version.
        virtual int                 version() const = 0;

        //  Given an instruction bit pattern finds the type of the instruction.
        //  Returns "nullptr" if the instruction type cannot be determinal (invalid bit pattern).
        virtual InstructionType *   findInstructionType(Instruction instruction) const = 0;
    };

    //////////
    //  The Cereon ISA V1
    class EMUONE_CEREON_EXPORT InstructionSetV1 : public InstructionSet
    {
        DECLARE_SINGLETON(InstructionSetV1)

        //////////
        //  InstructionSet
    public:
        virtual int                 version() const override;
        virtual InstructionType *   findInstructionType(Instruction instruction) const override;
    };
}

//  End of emuone-cereon/InstructionSets.hpp
