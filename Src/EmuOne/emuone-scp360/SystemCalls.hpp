//
//  emuone-scp360/SystemCalls.hpp
//
//  SCP system calls
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A generic SCP/360 "system call" made by a process
    class EMUONE_SCP360_EXPORT SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemCall)

        //////////
        //  Construction/destruction
    public:
        SystemCall(Process * process) : _process(process) { Q_ASSERT(_process != nullptr); }
        virtual ~SystemCall() {}

        //////////
        //  Operations
    public:
        Process *           process() const { return _process; }

        //////////
        //  Implementation
    private:
        Process *           _process;   //  ...that made the system call
    };

    //////////
    //  The "write to operator" system call
    class EMUONE_SCP360_EXPORT WriteToOperatorSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WriteToOperatorSystemCall)

        //////////
        //  Construction/destruction
    public:
        WriteToOperatorSystemCall(Process * process, const QByteArray & messageBytes)
            :   SystemCall(process), _messageBytes(messageBytes) {}
        virtual ~WriteToOperatorSystemCall() {}

        //////////
        //  Operations
    public:
        QByteArray          messageBytes() const { return _messageBytes; }

        //////////
        //  Implementation
    private:
        QByteArray          _messageBytes;  //  in EBCDIC!
    };
}

//  End of emuone-scp360/SystemCalls.hpp
