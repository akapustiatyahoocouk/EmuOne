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
    //  A generic SCP/360 "system call" made by a process.
    //  A SystemCall instance represents a SVC made by a running process
    //  along with all input parameters AND placeholder for output parameters.
    //  In a native (/360) code the SystemCall parameters are handled by the
    //  SystemCall including a "save area" for all 16 registers (and 4 FP ones).
    //  These are saved upon entry to the SVC handling routine; the system call
    //  handler will examine those "saved registers" that carry system call
    //  parameters and modify those "saved registers" that carry result back
    //  to the calling process.
    //  A new SystemCall instance is created immediately upon entry to the
    //  SVC handling routine.
    //  A new SystemCall instance is created immediately before return from
    //  the SVC handling routine (which will be "load old SVC PSW instruction).
    class EMUONE_SCP360_EXPORT SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemCall)

        //////////
        //  Construction/destruction
    public:
        explicit SystemCall(Process * process)
            :   _process(process) { Q_ASSERT(_process != nullptr); }
        virtual ~SystemCall() {}

        //////////
        //  Operations
    public:
        //  The Process which had made the system call.
        Process *           process() const { return _process; }

        //  True of the outcome of this system call is known (initially false).
        bool                outcomeKnown() const { return _outcomeKnown; }

        //  The outcome of this system call (MUST be known!)
        ErrorCode           outcome() const { Q_ASSERT(_outcomeKnown); return _outcome; }

        //  Sets the outcome of this system call. Can only be called ONCE.
        void                setOutcome(ErrorCode outcome)
        {
            Q_ASSERT(!_outcomeKnown);
            _outcomeKnown = true;
            _outcome = outcome;
        }

        //////////
        //  Implementation
    private:
        Process *const      _process;   //  ...that made the system call
        bool                _outcomeKnown = false;
        ErrorCode           _outcome = ErrorCode::ERR_UNK;
    };

    //////////
    //  A system call that does not involve the calling process "waiting" for anything.
    //  Such system calls can perform their work and return into SVC without context
    //  switching.
    class EMUONE_SCP360_EXPORT NowaitSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NowaitSystemCall)

        //////////
        //  Construction/destruction
    public:
        explicit NowaitSystemCall(Process * process) : SystemCall(process) {}
        virtual ~NowaitSystemCall() {}
    };

    //////////
    //  A system call that DOES involve the calling process "waiting" for anything.
    //  Such system calls will normally result in context switching to the next
    //  "ready" process.
    class EMUONE_SCP360_EXPORT WaitSystemCall : public SystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WaitSystemCall)

        //////////
        //  Construction/destruction
    public:
        explicit WaitSystemCall(Process * process) : SystemCall(process) {}
        virtual ~WaitSystemCall() {}
    };

    //////////
    //  A generic system call that involves data transfer
    class EMUONE_SCP360_EXPORT TransferDataSystemCall : public WaitSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(TransferDataSystemCall)

        //////////
        //  Construction/destruction
    public:
        TransferDataSystemCall(Process * processParam, ResourceHandle handleParam, util::Buffer * bufferParam)
            :   WaitSystemCall(processParam), handle(handleParam), buffer(bufferParam) { Q_ASSERT(buffer != nullptr); }
        virtual ~TransferDataSystemCall() {}

        //////////
        //  Properties
    public:
        //  The "handle" to the I/O Resource to write to
        ResourceHandle      handle;

        //  The buffer to write from; must not be deleted before the system call.
        util::Buffer *const buffer;

        //  The number of bytes actually transferred
        uint32_t            bytesTransferred = 0;
    };

    //////////
    //  The "write to operator" system call
    class EMUONE_SCP360_EXPORT WriteToOperatorSystemCall : public TransferDataSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WriteToOperatorSystemCall)

        //////////
        //  Construction/destruction
    public:
        WriteToOperatorSystemCall(Process * processParam, util::Buffer * bufferParam)
            :   TransferDataSystemCall(processParam, 0, bufferParam) {}
        virtual ~WriteToOperatorSystemCall() {}
    };

    //////////
    //  The "read from operator" system call
    class EMUONE_SCP360_EXPORT ReadFromOperatorSystemCall : public TransferDataSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReadFromOperatorSystemCall)

        //////////
        //  Construction/destruction
    public:
        ReadFromOperatorSystemCall(Process * processParam, util::Buffer * bufferParam)
            :   TransferDataSystemCall(processParam, 0, bufferParam) {}
        virtual ~ReadFromOperatorSystemCall() {}
    };

    //////////
    //  The "open file" system call
    class EMUONE_SCP360_EXPORT OpenFileSystemCall : public WaitSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(OpenFileSystemCall)

        //////////
        //  Construction/destruction
    public:
        OpenFileSystemCall(Process * processParam, const QString & fileNameParam, OpenFileFlags openFlagsParam,
                           uint32_t recordSizeParam, uint32_t blockSizeParam)
            :   WaitSystemCall(processParam), fileName(fileNameParam), openFlags(openFlagsParam), recordSize(recordSizeParam) , blockSize(blockSizeParam) {}
        virtual ~OpenFileSystemCall() {}

        //////////
        //  Properties
    public:
        const QString       fileName;
        const OpenFileFlags openFlags;
        const uint32_t      recordSize;
        const uint32_t      blockSize;

        //  The recipient for the "handle" to the newly open file
        uint16_t            handle = 0;
    };

    //////////
    //  The "write to file" system call
    class EMUONE_SCP360_EXPORT WriteToFileSystemCall : public TransferDataSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WriteToFileSystemCall)

        //////////
        //  Construction/destruction
    public:
        WriteToFileSystemCall(Process * processParam, ResourceHandle handleParam, util::Buffer * bufferParam)
            :   TransferDataSystemCall(processParam, handleParam, bufferParam) {}
        virtual ~WriteToFileSystemCall() {}
    };

    //////////
    //  The "write to file" system call
    class EMUONE_SCP360_EXPORT ReadFromFileSystemCall : public TransferDataSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReadFromFileSystemCall)

        //////////
        //  Construction/destruction
    public:
        ReadFromFileSystemCall(Process * processParam, ResourceHandle handleParam, util::Buffer * bufferParam)
            :   TransferDataSystemCall(processParam, handleParam, bufferParam) {}
        virtual ~ReadFromFileSystemCall() {}
    };

    //////////
    //  The "set environment variable value" system call
    class EMUONE_SCP360_EXPORT SetEnvironmentVariableValueSystemCall : public NowaitSystemCall
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SetEnvironmentVariableValueSystemCall)

        //////////
        //  Construction/destruction
    public:
        SetEnvironmentVariableValueSystemCall(Process * processParam, Process * targetProcessParam, const QString & nameParam, const QString & valueParam)
            :   NowaitSystemCall(processParam), targetProcess(targetProcessParam), name(nameParam), listValue{valueParam} {}
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
