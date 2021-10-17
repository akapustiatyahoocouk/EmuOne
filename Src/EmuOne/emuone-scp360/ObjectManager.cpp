//
//  emuone-scp360/ObjectManager.cpp
//
//  The scp360::ObjectManager class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
ObjectManager::ObjectManager(Scp * scp)
    :   _scp(scp)
{
    Q_ASSERT(_scp != nullptr);
}

ObjectManager::~ObjectManager()
{
    //  TODO destroy all Objects
}

//////////
//  Operations
ErrorCode ObjectManager::createPhysicalDevice(ibm360::Device * hardwareDevice, Device::Flags flags, DeviceDriver * driver, PhysicalDevice *& physicalDevice)
{
    if (hardwareDevice == nullptr || driver == nullptr)
    {
        physicalDevice = nullptr;
        return ErrorCode::ERR_PAR;
    }

    Object::Id id;
    ErrorCode err = _generateUniqueObjectId(id);
    if (err != ErrorCode::ERR_OK)
    {
        physicalDevice = nullptr;
        return err;
    }

    physicalDevice = new PhysicalDevice(_scp, id, hardwareDevice, flags, driver);
    _registerObject(physicalDevice);
    return ErrorCode::ERR_OK;
}

ErrorCode ObjectManager::createSegment(const QString & name, uint32_t size, Segment::Flags flags, uint32_t address, Segment *& segment)
{
    if (!Segment::isValidName(name))
    {
        segment = nullptr;
        return ErrorCode::ERR_PAR;
    }
    if (size > 16 * 1024 * 1024 ||
        address > 16 * 1024 * 1024 ||
        address + size > 16 * 1024 * 1024)
    {
        segment = nullptr;
        return ErrorCode::ERR_PAR;
    }
    if ((flags & Segment::Flags::All) != flags)
    {
        segment = nullptr;
        return ErrorCode::ERR_PAR;
    }

    Object::Id id;
    ErrorCode err = _generateUniqueObjectId(id);
    if (err != ErrorCode::ERR_OK)
    {
        segment = nullptr;
        return err;
    }

    segment = new Segment(_scp, id, name, size, flags, address);
    _registerObject(segment);
    return ErrorCode::ERR_OK;
}

ErrorCode ObjectManager::destroySegment(Segment * segment)
{
    if (segment == nullptr || segment->scp() != _scp ||
        _segments.contains(segment->id()) || _segments[segment->id()] != segment)
    {
        return ErrorCode::ERR_PAR;
    }
    if (segment->holdCount() != 0)
    {   //  OOPS! Can't!
        return ErrorCode::ERR_SUP;
    }

    //  TODO Destroy mappings/locks for the segment
    //  Destroy the segment itself
    _unregisterObject(segment);
    delete segment;
    return ErrorCode::ERR_OK;
}

ErrorCode ObjectManager::createEmulatedProcess(const QString & name, Process::Flags flags, Process * parent, EmulatedApplication * emulatedApplication, EmulatedProcess *& process)
{
    if (!Process::isValidName(name))
    {
        process = nullptr;
        return ErrorCode::ERR_PAR;
    }
    if ((flags & Process::Flags::All) != flags)
    {
        process = nullptr;
        return ErrorCode::ERR_PAR;
    }
    if (parent != nullptr && parent->scp() != _scp)
    {
        process = nullptr;
        return ErrorCode::ERR_PAR;
    }
    if (emulatedApplication == nullptr)
    {
        process = nullptr;
        return ErrorCode::ERR_PAR;
    }

    Object::Id id;
    ErrorCode err = _generateUniqueObjectId(id);
    if (err != ErrorCode::ERR_OK)
    {
        process = nullptr;
        return err;
    }

    process = emulatedApplication->createInstance(_scp, id, name, flags, parent);
    _registerObject(process);
    return ErrorCode::ERR_OK;
}

//////////
//  Implementation helpers
ErrorCode ObjectManager::_generateUniqueObjectId(Object::Id & id)
{   //  TODO improve perfrmance
    for (uint16_t candidateId = 1; candidateId <= 32767; candidateId++)
    {
        if (!_objects.contains(candidateId))
        {
            id = candidateId;
            return ErrorCode::ERR_OK;
        }
    }
    return ErrorCode::ERR_LIM;
}

void ObjectManager::_registerObject(Object * object)
{
    Q_ASSERT(object != nullptr);
    Q_ASSERT(object->scp() == _scp);
    Q_ASSERT(!_objects.contains(object->id()));

    _objects.insert(object->id(), object);

    if (Segment * segment = dynamic_cast<Segment*>(object))
    {
        _segments.insert(segment->id(), segment);
    }
    else if (SegmentMapping * segmentMapping = dynamic_cast<SegmentMapping*>(object))
    {
        _segmentMappings.insert(segmentMapping->id(), segmentMapping);
    }
    else if (SegmentLock * segmentLock = dynamic_cast<SegmentLock*>(object))
    {
        _segmentLocks.insert(segmentLock->id(), segmentLock);
    }
    else if (Process * process = dynamic_cast<Process*>(object))
    {
        _processes.insert(process->id(), process);
    }
    else if (Device * device = dynamic_cast<Device*>(object))
    {
        _devices.insert(device->id(), device);
        Q_ASSERT(!_devicesByName.contains(device->name()));
        _devicesByName.insert(device->name(), device);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void ObjectManager::_unregisterObject(Object * object)
{
    Q_ASSERT(object != nullptr);
    Q_ASSERT(object->scp() == _scp);
    Q_ASSERT(_objects.contains(object->id()) && _objects[object->id()] == object);

    _objects.remove(object->id());

    if (Segment * segment = dynamic_cast<Segment*>(object))
    {
        _segments.remove(segment->id());
    }
    else if (SegmentMapping * segmentMapping = dynamic_cast<SegmentMapping*>(object))
    {
        _segmentMappings.remove(segmentMapping->id());
    }
    else if (SegmentLock * segmentLock = dynamic_cast<SegmentLock*>(object))
    {
        _segmentLocks.remove(segmentLock->id());
    }
    else if (Process * process = dynamic_cast<Process*>(object))
    {
        _processes.remove(process->id());
    }
    else if (Device * device = dynamic_cast<Device*>(object))
    {
        _devices.remove(device->id());
        _devicesByName.remove(device->name());
    }
    else
    {
        Q_ASSERT(false);
    }
}

//  End of emuone-scp360/ObjectManager.cpp
