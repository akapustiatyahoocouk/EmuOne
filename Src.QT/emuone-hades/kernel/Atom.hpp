//
//  emuone-hades/kernel/Atom.hpp - The HADES OS kernel Atom
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::hades::kernel
{
    /// \class Atom emuone-hades/API.hpp
    /// \brief An arbitrary string with a unique system-wide [O]ID.
    class EMUONE_HADES_PUBLIC Atom final
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Atom)

        friend class Kernel;
        friend class ProcessInterestInAtom;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        Atom(Kernel * kernel, const Oid & oid, Identity * owner,
             const QString & value);
        virtual ~Atom();

        //////////
        //  Properties
        const QString       value;

        //////////
        //  Associations
        QMap<Process*, ProcessInterestInAtom*>
                            interestedProcesses;   //  Process -> interests in this Atom
    };

    /// \class ProcessInterestInAtom emuone-hades/API.hpp
    /// \brief An expression of a Process'es interest in an Atom.
    class EMUONE_HADES_PUBLIC ProcessInterestInAtom final
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ProcessInterestInAtom)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        ProcessInterestInAtom(Process * process, Atom * atom);
        virtual ~ProcessInterestInAtom();

        //////////
        //  Properties
        uint32_t        count = 0;

        //////////
        //  Associations
        Process *const  process;    //  "interested" in an Atom, counts as a "reference"
        Atom *const     atom;       //  in which the Process is "interested", counts as a "reference"
    };
}

//  End of emuone-hades/kernel/Atom.hpp
