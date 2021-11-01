//
//  emuone-scp360/Segment.cpp
//
//  The scp360::Segment class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
Segment::Segment(Scp * scp, Id id,
                 const QString & name, uint32_t size, Flags flags, uint32_t address)
    :   Object(scp, id),
        _name(name),
        _size(size),
        _flags(flags),
        _mappings(),
        _locks(),
        _contentLocation(new StorageContentLocation(address)),
        _holdCount(0)
{
    Q_ASSERT(Validator::isValidSegmentName(_name));
    Q_ASSERT(_size <= 16 * 1024 * 1024);
    Q_ASSERT(address <= 16 * 1024 * 1024 && address % 2048 == 0);
    Q_ASSERT(address + _size <= 16 * 1024 * 1024);
    Q_ASSERT((_flags & Flags::All) == _flags);
}

Segment::~Segment()
{
    //  TODO delete all _mappings(),
    //  TODO delete all _locks(),
    delete _contentLocation;
}

//////////
//  Operations

//////////
//  Helper functions
EMUONE_SCP360_EXPORT Segment::Flags scp360::operator & (Segment::Flags op1, Segment::Flags op2)
{
    return Segment::Flags(uint16_t(op1) & uint16_t(op2));
}

EMUONE_SCP360_EXPORT Segment::Flags scp360::operator | (Segment::Flags op1, Segment::Flags op2)
{
    return Segment::Flags(uint16_t(op1) | uint16_t(op2));
}

EMUONE_SCP360_EXPORT Segment::Flags & scp360::operator &= (Segment::Flags & op1, Segment::Flags op2)
{
    op1 = op1 & op2;
    return op1;
}

EMUONE_SCP360_EXPORT Segment::Flags & scp360::operator |= (Segment::Flags & op1, Segment::Flags op2)
{
    op1 = op1 | op2;
    return op1;
}

//  End of emuone-scp360/Segment.cpp
