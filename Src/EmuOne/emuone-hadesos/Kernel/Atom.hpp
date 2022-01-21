//
//  emuone-hadesos/Kernel/Atom.hpp
//
//  The HADES OS "component"atom" ADT
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  A HADES OS "Atom" is a system-wide mapping of a fixed-size "ID" to a String
        class EMUONE_HADESOS_EXPORT Atom final : public Object
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Atom)

            //////////
            //  Construction/destruction
        public:
            Atom(Kernel * kernel, ObjectId id, const QString & string);
            virtual ~Atom();

            //////////
            //  Operations
        public:
            //  Returns the string of this atom
            QString             string() const { return _string; }

            //////////
            //  Implementation
        private:
            const QString       _string;

            AtomLockList        _locks; //  all "AtomLock"s that refer to this Atom
        };

        //////////
        //  An "atom lock" represents an interest of some process in some atom.
        //  The "lock count" says how many times that process has expressed such interest.
        //  "Atom locks" are owned by their associated "Processes" and are destroyed when
        //  the "Process" in question is destroyed.
        class EMUONE_HADESOS_EXPORT AtomLock final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AtomLock)

            //////////
            //  Construction/destruction
        public:
            AtomLock();
            ~AtomLock();

            //////////
            //  Implementation
        private:
            Process *const      _process;   //  ..."interested" in an atom
            Atom *const         _atom;      //  ...that the process is interested in
            uint32_t            _count;     //  ...of times the "_process" has expressed an interest in an "_atom"
        };
    }
}

//  End of emuone-hadesos/Kernel/Atom.hpp
