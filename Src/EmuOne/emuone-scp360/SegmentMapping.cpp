//
//  emuone-scp360/SegmentMapping.cpp
//
//  The scp360::SegmentMapping class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
SegmentMapping::SegmentMapping(Scp * scp, Id id, Segment * segment, Process * process)
    :   Object(scp, id),
        _segment(segment),
        _process(process),
        _count(0)
{
    Q_ASSERT(_segment != nullptr && _segment->scp() == scp);
    Q_ASSERT(_process != nullptr && _process->scp() == scp);
}

SegmentMapping::~SegmentMapping()
{
}

//  End of emuone-scp360/SegmentMapping.cpp
