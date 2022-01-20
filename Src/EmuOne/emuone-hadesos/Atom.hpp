//
//  emuone-hadesos/Atom.hpp
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
        class EMUONE_HADESOS_EXPORT Atom final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Atom)

            //////////
            //  Types
        public:
            using Id = uint32_t;    //  A system-wide unique atom ID

            //////////
            //  Construction/destruction
        public:
            Atom();
            ~Atom();

            //////////
            //  Operations
        public:
            //  Returns the ID of this atom
            Id                  id() const { return _id; }

            //  Returns the string of this atom
            QString             string() const { return _string; }

            //////////
            //  Implementation
        private:
            const Id            _id;
            const QString       _string;

            AtomLockList        _locks; //  all "AtomLock"s that refer to this Atom
        };

        //////////
        //  An "atom lock" represents an interest of some process in some atom.
        //  The "lock count" says how many times that process has expressed such interest.
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

//  End of emuone-hadesos/Atom.hpp
