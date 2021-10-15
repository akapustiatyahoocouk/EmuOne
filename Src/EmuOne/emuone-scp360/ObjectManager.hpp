//
//  emuone-scp360/ObjectManager.hpp
//
//  A SCP/360 Object Manager
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A SCP/360 Object Manager
    class EMUONE_SCP360_EXPORT ObjectManager final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ObjectManager)

        //////////
        //  Construction/destruction
    public:
        explicit ObjectManager(Scp * scp);
        ~ObjectManager();

        //////////
        //  Operations
    public:
        ErrorCode           createSegment(const QString & name, uint32_t size, Segment::Flags flags, uint32_t address, Segment *& segment);
        ErrorCode           destroySegment(Segment * segment);

        ErrorCode           createEmulatedProcess(const QString & name, Process::Flags flags, Process * parent, EmulatedApplication * emulatedApplication, EmulatedProcess *& process);

        //////////
        //  Implementation
    private:
        Scp *const          _scp;   //  ...to which this subsystem belongs

        //  Primary object cache
        QMap<Object::Id, Object*>   _objects;

        //  Secondary object caches (by object type)
        QMap<Object::Id, Segment*>  _segments;
        QMap<Object::Id, SegmentMapping*>   _segmentMappings;
        QMap<Object::Id, SegmentLock*>      _segmentLocks;
        QMap<Object::Id, Process*>          _processes;

        //  Helpers
        ErrorCode           _generateUniqueObjectId(Object::Id & id);
        void                _registerObject(Object * object);
        void                _unregisterObject(Object * object);
    };
}

//  End of emuone-scp360/ObjectManager.hpp
