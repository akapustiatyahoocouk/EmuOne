//
//  emuone-scp360/EmulatedProcess.hpp
//
//  The SCP/360 emulated process
//
//////////
#pragma once

namespace scp360
{

    //////////
    //  A SCP/360 application Process that runs C++ code
    class EMUONE_SCP360_EXPORT EmulatedProcess : public Process
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EmulatedProcess)

        friend class Scp;

        //////////
        //  Construction/destruction - sub-classes only
    protected:
        EmulatedProcess(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent);
        virtual ~EmulatedProcess();

        //////////
        //  Operations
    public:
        virtual EmulatedApplication *   application() const = 0;
        void                    start();
        void                    stop(ErrorCode exitCode);
    protected:
        virtual ErrorCode       run() = 0;

        //////////
        //  System calls for an emulated process (all synchronous!)
    public:
        class EMUONE_SCP360_EXPORT SystemCalls final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemCalls)

            friend class EmulatedProcess;

            //////////
            //  Construction/destruction - from friends only
        private:
            explicit SystemCalls(EmulatedProcess * emulatedProcess) : _emulatedProcess(emulatedProcess) {}
            ~SystemCalls() {}

            //////////
            //  Operations
        public:
            //  Makes the specified "systemCall", returning only after the
            //  system call has completed and its outcome is known.
            //  The caller (who is the one that created the systemCall) is
            //  then responsible for deleting the "systemCall".
            ErrorCode               makeSystemCall(SystemCall * systemCall);

            //  Writes a single message (record) to the "operator console".
            //  If the "text" is too long, it is truncated at the right edge to fit to
            //  a single record (e.g. a single 80-column line on IBM 2741, etc.)
            //  Returns when the writing has completed one way or another.
            ErrorCode               writeToOperator(const QString & text);

            //  Opens the file with the specified name.
            //  I/O redirection via environment is performed automatically.
            //  Upon success returns ERR_OK and stores a valid "fileHandle".
            //  Upon failure returns a non-ERR_OK error code.
            ErrorCode               openFile(const QString & fileName, OpenFileFlags openFlags,
                                             uint32_t recordSize, uint32_t blockSize, uint16_t & fileHandle);

            ErrorCode               writeToFile(uint16_t handle, const void * data, uint32_t bytesToWrite,
                                                uint32_t & bytesWritten);
            ErrorCode               writeToFile(uint16_t handle, const QString & data,
                                                uint32_t & bytesWritten);
            ErrorCode               readFromFile(uint16_t handle, void * data, uint32_t bytesToRead,
                                                uint32_t & bytesRead);
            ErrorCode               readFromFile(uint16_t handle, QString & data, uint32_t bytesToRead,
                                                uint32_t & bytesRead);

/*
    7.2.2	SC#EVCNT – Count Environment Variable Values
    This system call allows the process to determine the number of values that a specified variable in its environment has. As a special case, it can also be used to determine if the specified environment variable exists at all.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVCNT (X’00020101’)
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure. Typically ERR#NOF, which means that the specified environment variable does not exist.
    •	R0 = the number of values the specified environment variable has; always 1 (scalar) or more upon success; undefined upon failure.
    Comments	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    7.2.3	SC#EVGTL – Get Environment Variable Value (List)
    This system call allows the process to retrieve an individual value from the list of values associated with a given variable in its environment.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVGTL (X’00020102’)
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    •	R4 = the 0-based index of the value to retrieve.
    •	R5 = the address of the buffer to store the value to.
    •	R6 = the length of the buffer to store the value to.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure.
    •	R0 = the number of characters actually stored into the supplied buffer (upon success); undefined (upon failure).
    •	Other registers are unchanged.
    Comment	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    •	If the environment variable in question does not exist, or the index exceeds the number of values it has, R15 will be ERR#NOF upon return.
    •	If the specified buffer is too small to hold the entire value, only the initial portion of the value (as much as fits into the buffer) will be stored and R15 will be ERR#BTS upon return.
    •	If the specified buffer is larger than the value, only the initial portion of the buffer is filled, and the rest of the buffer is not affected; the number of characters actually stored is returned in R0.
    */
        ErrorCode               getEnvironmentVariableValueScalar(const QString & name, const QString & scalarValue);

    /*
    7.2.4	SC#EVGTS – Get Environment Variable Value (Scalar)
    This system call allows the process to retrieve the complete value associated with a given variable in its environment.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVGTS
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    •	R4 = the address of the buffer to store the value to.
    •	R5 = the length of the buffer to store the value to.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure.
    •	R0 = the number of characters actually stored into the supplied buffer (upon success); undefined (upon failure).
    •	Other registers are unchanged.
    Comment	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    •	If the environment variable in question does not exist, or the index exceeds the number of values it has, R15 will be ERR#NOF upon return.
    •	If the environment variable has a list of more than 1 values, the string stored into the buffer will contain all these values separated by commas and enclosed in parentheses, e.g. (VAL1,VAL2,VAL3). The system call will fail unless the buffer is large enough to hold the entire list.
    •	If the specified buffer is too small to hold the entire value, only the initial portion of the value (as much as fits into the buffer) will be stored and R15 will be ERR#BTS upon return.
    •	If the specified buffer is larger than the value, only the initial portion of the buffer is filled, and the rest of the buffer is not affected; the number of characters actually stored is returned in R0.



    SC#EVSTL EQU   X'00020104'  SET ENVIRONMENT VARIABLE VALUE (LIST)
    */
            ErrorCode               setEnvironmentVariableValue(const QString & name, const QString & scalarValue);
            ErrorCode               setEnvironmentVariableValue(Process * targetProcess, const QString & name, const QString & scalarValue);
    /*
    SC#EVADL EQU   X'00020106'  ADD TO ENVIRONMENT VARIABLE VALUE (LIST)
    SC#EVDEL EQU   X'00020107'  DELETE ENVIRONMENT VARIABLE
    */

            //////////
            //  Implementation
        private:
            EmulatedProcess *const  _emulatedProcess;
        };
        SystemCalls             systemCalls;

        //////////
        //  Operations
    public:
        void                    markSystemCallComplete();
        ResourceHandle          sysinHandle() const { return _sysinHandle; }
        ResourceHandle          sysoutHandle() const { return _sysoutHandle; }

        //////////
        //  Implementation
    private:
        QSemaphore              _systemCallCompletionSemaphore;
        ResourceHandle          _sysinHandle = 0;
        ResourceHandle          _sysoutHandle = 0;

        //  Helpers
        void                    _openStdIo();

        //////////
        //  Threads
    private:
        //  Each EmulatedProcess runs on a separate worker thread
        class _WorkerThread : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Construction/destruction
        public:
            explicit _WorkerThread(EmulatedProcess * emulatedProcess);
            virtual ~_WorkerThread();

            //////////
            //  QThread
        public:
            virtual void    run() override;

            //////////
            //  Implementation
        private:
            EmulatedProcess *const  _emulatedProcess;
        };
        _WorkerThread *     _workerThread = nullptr;
    };

}

//  End of emuone-scp360/Objects.hpp
