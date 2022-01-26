//
//  emuone-hadesos-cereon/MemoryManager.hpp
//
//  HadesOS/Cereon Memory Manager
//
//////////

namespace hadesos
{
    namespace cereon
    {
        //////////
        //  The HADES OS / Cereon Memory Manager
        class EMUONE_HADESOS_CEREON_EXPORT MemoryManager : public virtual hadesos::kernel::IMemoryManager
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryManager)

            //////////
            //  Construction/destruction
        public:
            MemoryManager();
            virtual ~MemoryManager();
        };
    }
}

//  End of emuone-hadesos-cereon/MemoryManager.hpp
