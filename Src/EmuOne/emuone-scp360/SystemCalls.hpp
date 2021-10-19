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
        //  Construction/destruction
    public:
        OpenFileSystemCall(Process * process, const QString & fileName, OpenFileFlags openFlags,
                           uint32_t recordSize, uint32_t blockSize)
            :   SystemCall(process), fileName(fileName), openFlags(openFlags), recordSize(recordSize) , blockSize(blockSize) {}
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
