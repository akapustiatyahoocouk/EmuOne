//
//  emuone-hadesos/Kernel/Identity.hpp
//
//  The HADES OS "Identity" ADT
//
//////////
#pragma once

namespace hadesos
{
    namespace kernel
    {
        //////////
        //  A generic "identity" that identifies a client
        class EMUONE_HADESOS_EXPORT Identity : public Object
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Identity)

            //////////
            //  Construction/destruction
        public:
            Identity(Kernel * kernel, ObjectId id, Identity * owner,
                     const QString & name, bool active);
            virtual ~Identity();

            //////////
            //  Operations
        public:
            //  The "name" of this identity.
            QString             name() const { return _name; }

            //  The "active" status of this identity - "inactive" identities do not grant
            //  any access controls; an "inactive" group acts as of "nobody is a member of it".
            bool                active() const { return _active; }

            //////////
            //  Implementation
        private:
            QString             _name;
            bool                _active;
            GroupIdentityList   _groups = {};   //  ...to which this identitity belongs; no loops are allowed
        };

        //////////
        //  A "system identity" represents the Hades OS kernel
        class EMUONE_HADESOS_EXPORT SystemIdentity final : public Identity
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemIdentity)

            //////////
            //  Construction/destruction
        public:
            SystemIdentity(Kernel * kernel, ObjectId id);
            virtual ~SystemIdentity();
        };

        //////////
        //  A "user identity" represents a user who can perform interactive (or relote) login.
        class EMUONE_HADESOS_EXPORT UserIdentity final : public Identity
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UserIdentity)

            //////////
            //  Construction/destruction
        public:
            UserIdentity(Kernel * kernel, ObjectId id, const QString & name, bool active,
                         const QString & password);
            virtual ~UserIdentity();

            //////////
            //  Implementation
        private:
            QString             _passwordHash;  //  The SHA-1 hash of the user's password
        };

        //////////
        //  A "synthetic identity" represents a system service.
        class EMUONE_HADESOS_EXPORT SyntheticIdentity final : public Identity
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SyntheticIdentity)

            //////////
            //  Construction/destruction
        public:
            SyntheticIdentity(Kernel * kernel, ObjectId id, const QString & name, bool active,
                              const QString & password);
            virtual ~SyntheticIdentity();

            //////////
            //  Implementation
        private:
            QString             _passwordHash;  //  The SHA-1 hash of the user's password
        };

        //////////
        //  A "group identity" represents a group (or tree) of other identities.
        class EMUONE_HADESOS_EXPORT GroupIdentity final : public Identity
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(GroupIdentity)

            //////////
            //  Construction/destruction
        public:
            GroupIdentity(Kernel * kernel, ObjectId id, const QString & name, bool active);
            virtual ~GroupIdentity();

            //////////
            //  Implementation
        private:
            IdentityList        _members;   //  All member identities; no loops are allowed
        };
    }
}

//  End of emuone-hadesos/Kernel/Identity.hpp
