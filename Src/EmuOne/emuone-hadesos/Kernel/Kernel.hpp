//
//  emuone-hadesos/Kernel/Kernel.hpp
//
//  The HADES OS Kernel
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  A HADES OS Kernel "resulot code" - any kernel service will return one
        enum ErrorCode : uint32_t
        {
            Success = 0x00000000,

            //  Misc
            Unknown = 0xFFFFFFFF
        };

        //////////
        //  A HADES OS Kernel
        class EMUONE_HADESOS_EXPORT Kernel final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Kernel)

            //////////
            //  Construction/destruction
        public:
            Kernel(IMemoryManager * memoryManager);
            ~Kernel();

            //////////
            //  Operations
        public:
            //  Checks whether the kernel's state is "locked" by the currently executing thread
            bool                    isStateLocked() const { return _stateGuard.isLockedByCurrentThread(); }

            //////////
            //  Implementation
        private:
            mutable util::Mutex     _stateGuard = {};   //  ...used for all access to kernel data structures

            //////////
            //  Object tables
        private:
            //  Mandatory kernel subsystems
            IMemoryManager *        _memoryManager;

            //  The primary object table
            QMap<ObjectId, Object*> _objects = {};              //  all kernel objects that are currently live
            QQueue<ObjectId>        _recycledObjectIds = {};    //  IDs of destroyed kernel objects, ready for quick recycling

            //  Secondary object tables (for speeding up object lookup)
            QMap<QString, Identity*>_mapNamesToIdentities = {}; //  identity name -> identity
            QMap<QString, Atom*>    _mapAtomStringsToAtoms = {};//  atom string -> live atom

            //  "Special" kernel objects
            SystemIdentity *        _systemIdentity = nullptr;

            //////////
            //  Object table helpers
        private:
            ObjectId                _generateUnusedObjectId();
            SystemIdentity *        _createSystemIdentity();
        };
    }
}

//  End of emuone-hadesos/Kernel/Kernel.hpp
