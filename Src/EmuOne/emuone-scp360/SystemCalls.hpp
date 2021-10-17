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
}

//  End of emuone-scp360/SystemCalls.hpp
