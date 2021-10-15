//
//  emuone-scp360/SegmentLock.cpp
//
//  The scp360::SegmentLock class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
SegmentLock::SegmentLock(Scp * scp, Id id, Segment * segment, Process * process)
    :   Object(scp, id),
        _segment(segment),
        _process(process),
        _count(0)
{
    Q_ASSERT(_segment != nullptr && _segment->scp() == scp);
    Q_ASSERT(_process != nullptr && _process->scp() == scp);
}

SegmentLock::~SegmentLock()
{
}

//  End of emuone-scp360/SegmentLock.cpp
