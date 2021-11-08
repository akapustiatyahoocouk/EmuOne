//
//  emuone-cereon/InstructionSets.cpp
//
//  cereon::InstructionSet classes implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  InstructionSetV1
IMPLEMENT_SINGLETON(InstructionSetV1)
InstructionSetV1::InstructionSetV1() {}
InstructionSetV1::~InstructionSetV1() {}

int InstructionSetV1::version() const
{
    return 1;
}

InstructionType * InstructionSetV1::findInstructionType(Instruction /*instruction*/) const
{
    return nullptr;
}

//  End of emuone-cereon/InstructionSets.cpp
