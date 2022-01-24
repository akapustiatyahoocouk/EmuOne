//
//  emuone-hadesos/Kernel/Object.hpp
//
//  A generic "kernel object" managed by the Hades OS kernel
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        using ObjectId = uint32_t;
        static const ObjectId MinObjectId = static_cast<ObjectId>(1);
        static const ObjectId MaxObjectId = static_cast<ObjectId>(0xFFFFFFFE);
        static const ObjectId InvalidObjectId = static_cast<ObjectId>(0);

        //////////
        //  A generic "kernel object" managed by the Hades OS kernel
        class EMUONE_HADESOS_EXPORT Object
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

            //////////
            //  Construction/destruction
        public:
            Object(Kernel * kernel, ObjectId id, Identity * owner);
            virtual ~Object();

            //////////
            //  Operations
        public:
            Kernel *            kernel() const { return _kernel; }
            ObjectId            id() const { return _id; }

            //////////
            //  Implementation
        private:
            Kernel *const       _kernel;    //  ...managing this object
            const ObjectId      _id;

            Identity *          _owner;
        };
    }
}

//  End of emuone-hadesos/Kernel/Object.hpp
