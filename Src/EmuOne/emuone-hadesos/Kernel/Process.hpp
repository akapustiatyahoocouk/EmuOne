//
//  emuone-hadesos/Kernel/Process.hpp
//
//  The HADES OS kernel process
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  The HADES OS kernel process
        class EMUONE_HADESOS_EXPORT Process final : public Object
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Process)

            //////////
            //  Construction/destruction
        public:
            Process(Kernel * kernel, ObjectId id);
            virtual ~Process();
        };
    }
}

//  End of emuone-hadesos/Kernel/Process.hpp
