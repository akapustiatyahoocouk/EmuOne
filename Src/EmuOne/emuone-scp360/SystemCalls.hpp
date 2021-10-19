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
        bool                outcomeKnown() const { return _outcomeKnown; }
        ErrorCode           outcome() const { return _outcome; }
        void                setOutcome(ErrorCode outcome)
        {
            Q_ASSERT(!_outcomeKnown);
            _outcomeKnown = true;
            _outcome = outcome;
        }

        //////////
        //  Implementation
    private:
        Process *           _process;   //  ...that made the system call
        bool                _outcomeKnown = false;
        ErrorCode           _outcome = ErrorCode::ERR_UNK;
    };

    //////////
    //  The "write to operator" system call
    class EMUONE_SCP360_EXPORT WriteToOperatorSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WriteToOperatorSystemCall)

        //////////
        //  Constants
    public:
        //  The system call code (places into R0 before "SVC 10"
        static const uint32_t   Code = 0x00010002;

        //////////
        //  Construction/destruction
    public:
        WriteToOperatorSystemCall(Process * process, util::Buffer * buffer)
            :   SystemCall(process), buffer(buffer) {}
        virtual ~WriteToOperatorSystemCall() {}

        //////////
        //  Properties
    public:
        //  The buffer to write from; must not be deleted before the system call.
        util::Buffer *const buffer;
    };

    //////////
    //  The "open file" system call
    class EMUONE_SCP360_EXPORT OpenFileSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(OpenFileSystemCall)

        //////////
        //  Constants
    public:
        //  The system call code (places into R0 before "SVC 10"
        static const uint32_t   Code = 00010003;

        //  Open flags
        enum class OpenFlags : uint16_t
        {
            FixedUnblockedRecords       = 0x1,      //  FIXED UNBLOCKED RECORD FORMAT
            FixedBlockedRecords         = 0x2,      //  FIXED BLOCKED RECORD FORMAT
            VariableUnblockedRecords    = 0x3,      //  VARIABLE UNBLOCKED RECORD FORMAT
            VariableBlockedRecords      = 0x4,      //  VARIABLE BLOCKED RECORD FORMAT
            UndefinedRecords            = 0x5,      //  UNDEFINED RECORD FORMAT

            ReadOnly                    = 0x10,     //  OPEN FILE FOR INPUT
            WriteOnly                   = 0x20,     //  OPEN FILE FOR OUTPUT
            ReadWrite                   = 0x30,     //  OPEN FILE FOR INPUT AND OUTPUT (UPDATE)

            BinaryMode                  = 0x100,    //  OPEN FILE FOR BINARY I/O
            TextMode                    = 0x200,    //  OPEN FILE FOR TEXTUAL I/O

            SequentialAccess            = 0x1000,   //  OPEN FILE FOR SEQUENTIAL ACCESS
            IndexedSeqentialAccess      = 0x2000,   //  OPEN FILE FOR INDEXED-SEQUENTIAL ACCESS
            DirectAccess                = 0x3000    //  OPEN FILE FOR DIRECT ACCESS
        };

        //////////
        //  Construction/destruction
    public:
        OpenFileSystemCall(Process * process, const QString & fileName, OpenFlags openFlags,
                           uint32_t recordSize, uint32_t blockSize)
            :   SystemCall(process), fileName(fileName), openFlags(openFlags), recordSize(recordSize) , blockSize(blockSize) {}
        virtual ~OpenFileSystemCall() {}

        //////////
        //  Properties
    public:
        const QString       fileName;
        const OpenFlags     openFlags;
        const uint32_t      recordSize;
        const uint32_t      blockSize;
    };

    EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags operator & (OpenFileSystemCall::OpenFlags op1, OpenFileSystemCall::OpenFlags op2);
    EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags operator | (OpenFileSystemCall::OpenFlags op1, OpenFileSystemCall::OpenFlags op2);
    EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags & operator &= (OpenFileSystemCall::OpenFlags & op1, OpenFileSystemCall::OpenFlags op2);
    EMUONE_SCP360_EXPORT OpenFileSystemCall::OpenFlags & operator |= (OpenFileSystemCall::OpenFlags & op1, OpenFileSystemCall::OpenFlags op2);

    //////////
    //  The "set environment variable value" system call
    class EMUONE_SCP360_EXPORT SetEnvironmentVariableValueSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SetEnvironmentVariableValueSystemCall)

        //////////
        //  Construction/destruction
    public:
        SetEnvironmentVariableValueSystemCall(Process * process, Process * targetProcess, const QString & name, const QString & value)
            :   SystemCall(process), targetProcess(targetProcess), name(name), listValue{value} {}
        virtual ~SetEnvironmentVariableValueSystemCall() {}

        //////////
        //  Properties
    public:
        Process *const          targetProcess;
        const QString           name;
        const QStringList       listValue;
    };
}

//  End of emuone-scp360/SystemCalls.hpp
