//
//  emuone-core/Exceptions.hpp
//
//  VM core exceptions
//
//////////
#pragma once

namespace core
{
    //////////
    //  A generic exception thrown when a VM service fails
    class EMUONE_CORE_EXPORT VirtualApplianceException : public QException
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualApplianceException)

        //////////
        //  Construction/destruction
    public:
        explicit VirtualApplianceException(const QString & message)
            :   _message(message) {}

        //////////
        //  Operations
    public:
        QString             getMessage() const { return _message; }

        //////////
        //  Implementation
    private:
        QString             _message;
    };
}

//  End of emuone-core/Exceptions.hpp
