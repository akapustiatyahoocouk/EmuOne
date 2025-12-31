//
//  emuone-hades/kernel/Identity.hpp - The HADES OS Kernel identities
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
    /// \class Identity emuone-hades/API.hpp
    /// \brief An identity of the worker.
    class EMUONE_HADES_PUBLIC Identity
        :   public Object
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Identity)

        friend class Object;
        friend class SystemIdentity;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Construction/destruction
        Identity(Kernel * kernel, const Oid & oid);
        virtual ~Identity();

        //////////
        //  Associations
        Objects         ownedObjects;   //  count as references

        //////////
        //  Operations
        virtual QString name() const = 0;
    };

    /// \class SystemIdentity emuone-hades/API.hpp
    /// \brief An identity of the HADES kernel itself.
    class EMUONE_HADES_PUBLIC SystemIdentity final
        :   public Identity
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemIdentity)

        friend class Kernel;

        //////////
        //  All members are private - for Kernel only
    private:

        //////////
        //  Constants
        inline static const QString Name = "System";   //  constructed ONCE

        //////////
        //  Construction/destruction
        SystemIdentity(Kernel * kernel, const Oid & oid);
        virtual ~SystemIdentity();

        //////////
        //  Identity
        virtual QString name() const override { return Name; }
    };
}

//  End of emuone-hades/kernel/Identity.hpp
