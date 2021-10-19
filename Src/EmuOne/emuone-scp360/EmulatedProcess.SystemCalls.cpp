//
//  emuone-scp360/Scp.SystemCalls.cpp
//
//  The scp360::Scp::SystemCalls class implementation (system calls)
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  System calls for an emulated process
ErrorCode EmulatedProcess::SystemCalls::makeSystemCall(SystemCall * systemCall)
{
    Q_ASSERT(QThread::currentThread() == _emulatedProcess->_workerThread);
    Q_ASSERT(systemCall != nullptr);
    Q_ASSERT(systemCall->process() == _emulatedProcess);
    Q_ASSERT(!_emulatedProcess->_systemCallCompletionSemaphore.available());

    //  We want to  send a "system call" event to SCP...
    ErrorCode err = _emulatedProcess->scp()->makeSystemCall(systemCall);
    if (err != ErrorCode::ERR_OK)
    {   //  The SCP has refused the system call request
        return err;
    }
    //  ...and wait until the system call has been completed
    _emulatedProcess->_systemCallCompletionSemaphore.acquire();
    Q_ASSERT(systemCall->outcomeKnown());
    if (_emulatedProcess->_workerThread->isInterruptionRequested())
    {   //  Stop at once!
        throw ErrorCode::ERR_UNK;
    }
    return systemCall->outcome();
}

ErrorCode EmulatedProcess::SystemCalls::writeToOperator(const QString & text)
{
    Q_ASSERT(QThread::currentThread() == _emulatedProcess->_workerThread);

    //  The "operator console" is a text device that expects EBCDIC bytes.
    util::ByteArrayBuffer buffer;
    std::unique_ptr<util::CharacterSet::Encoder> encoder(util::Cp037CharacterSet::getInstance()->createEncoder());
    encoder->encode(text, buffer.data);

    //  Issue "SVC"
    WriteToOperatorSystemCall systemCall(_emulatedProcess, &buffer);
    return makeSystemCall(&systemCall);
}

ErrorCode EmulatedProcess::SystemCalls::openFile(const QString & fileName, OpenFileFlags openFlags,
                                                 uint32_t recordSize, uint32_t blockSize, uint16_t & fileHandle)
{
    Q_ASSERT(QThread::currentThread() == _emulatedProcess->_workerThread);

    //  Issue "SVC"
    OpenFileSystemCall systemCall(_emulatedProcess, fileName, openFlags, recordSize, blockSize);
    ErrorCode err = makeSystemCall(&systemCall);
    if (err != ErrorCode::ERR_OK)
    {   //  OOPS! Something wrong!
        return err;
    }
    fileHandle = systemCall.handle;
    return ErrorCode::ERR_OK;
}

ErrorCode EmulatedProcess::SystemCalls::setEnvironmentVariableValue(const QString & name, const QString & scalarValue)
{
    Q_ASSERT(QThread::currentThread() == _emulatedProcess->_workerThread);

    //  Issue "SVC"
    SetEnvironmentVariableValueSystemCall systemCall(_emulatedProcess, _emulatedProcess, name, scalarValue);
    return makeSystemCall(&systemCall);
}

ErrorCode EmulatedProcess::SystemCalls::setEnvironmentVariableValue(Process * targetProcess, const QString & name, const QString & scalarValue)
{
    Q_ASSERT(QThread::currentThread() == _emulatedProcess->_workerThread);

    //  Issue "SVC"
    SetEnvironmentVariableValueSystemCall systemCall(_emulatedProcess, targetProcess, name, scalarValue);
    return makeSystemCall(&systemCall);
}

//  End of emuone-scp360/Scp.SystemCalls.cpp
