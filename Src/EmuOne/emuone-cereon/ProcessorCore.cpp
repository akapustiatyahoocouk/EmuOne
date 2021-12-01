//
//  emuone-cereon/ProcessorCore.cpp
//
//  cereon::ProcessorCore class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
ProcessorCore::ProcessorCore(Processor * processor, uint8_t coreId, bool primary)
    :   registers(),
        _processor(processor),
        _coreId(coreId),
        _primary(primary)
{
    Q_ASSERT(_processor != nullptr);
}

ProcessorCore::~ProcessorCore()
{
}

//////////
//  Operations
void ProcessorCore::initialise()
{
}

void ProcessorCore::start()
{   //  Perform bootstrapping

    //  1.  The W flag of each DMA channel’s $state register is set to 0.
    //      Done at the Processor level

    //  2.  All registers of all processors are set to 0, with the exception of registers
    //      explicitly specified below as being set to something else.
    for (int i = 0; i < 32; i++)
    {
        registers.r[i] = 0;
        registers.c[i] = 0;
        registers.d[i] = 0;
        registers.m[i] = 0;
        registers.flags = 0;
    }

    //  3.  In every Cereon system, there is exactly one processor set up as a primary
    //      processor. If the system has more than one processor, all remaining processors
    //      are set up as secondary processors. This processor type setup is hardwired and
    //      cannot be changed. If the primary processor has more than one core, one of
    //      these cores is hardwired as a primary core.

    //  4.  For each processor, whether primary or secondary, the special bootstrap IP
    //      value is hardwired. This value is copied to $ip register.
    registers.ip = _processor->bootstrapIp();

    //  5.  For each processor, the K flag of $state if set to 1.
    registers.state.setKernelMode();

    //  6.  For each processor, the B flag of $state is set to reflect the default byte
    //      ordering. Whether this flag can be changed later or not depends on the
    //      processor model.
    registers.state.setByteOrder(_processor->byteOrder());

    //  7.  For the primary core of the primary processor, bit 31 of $state is set to 1,
    //      thus allowing HARDWARE interrupts.
    if (primary() && _processor->primary())
    {
        registers.state.enableHardwareInterrupt();
    }

    //  8.  For all secondary processor cores, bits 30 and 31 of $state is set to 1, thus
    //      allowing EXTERNAL and HARDWARE interrupts.
    if (!primary() || !_processor->primary())
    {
        registers.state.enableHardwareInterrupt();
        registers.state.enableExternalInterrupt();
    }

    //  9.  For all secondary processor cores, $ip is copied to $iha.ext and $state
    //      is copied to $ihstate.ext. After the copy, the W flag of $ihstate.ext
    //      is set to 1.
    if (!primary() || !_processor->primary())
    {
        registers.ihaExt = registers.ip;
        registers.ihstateExt = static_cast<uint64_t>(registers.state);
        registers.ihstateExt.setWorkingMode();
    }

    //  10. For the primary processor, the W flag of $state is set to 1. This effectively
    //      starts the primary processor.
    if (primary() && _processor->primary())
    {
        registers.state.setWorkingMode();
    }
}

void ProcessorCore::stop() noexcept
{
}

void ProcessorCore::deinitialise() noexcept
{
}

void ProcessorCore::onClockTick()
{
}

//  End of emuone-cereon/ProcessorCore.cpp
