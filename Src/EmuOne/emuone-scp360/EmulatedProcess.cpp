//
//  emuone-scp360/EmulatedProcess.cpp
//
//  The scp360::EmulatedProcess class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
EmulatedProcess::EmulatedProcess(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent)
    :   Process(scp, id, name, flags, parent)
{
}

EmulatedProcess::~EmulatedProcess()
{
}

//////////
//  Operations
void EmulatedProcess::start()
{
    Q_ASSERT(_state == State::Prepared);
    _workerThread = new _WorkerThread(this);
    _workerThread->start();
}

//////////
//  System calls for an emulatedprocess
ErrorCode EmulatedProcess::makeSystemCall(SystemCall * systemCall)
{
    Q_ASSERT(systemCall != nullptr);
    Q_ASSERT(systemCall->process() == this);
    Q_ASSERT(!_systemCallCompletionSemaphore.available());

    //  We want to  send a "system call" event to SCP...
    ErrorCode err = scp()->makeSystemCall(systemCall);
    if (err != ErrorCode::ERR_OK)
    {   //  The SCP has refused the system call request
        return err;
    }
    //  ...and wait until the system call has been completed
    _systemCallCompletionSemaphore.acquire();
    Q_ASSERT(systemCall->outcomeKnown());
    return systemCall->outcome();
}

ErrorCode EmulatedProcess::writeToOperator(const QString & text)
{
    //  The "operator console" is a text device that expects EBCDIC bytes.
    util::ByteArrayBuffer buffer;
    std::unique_ptr<util::CharacterSet::Encoder> encoder(util::Cp037CharacterSet::getInstance()->createEncoder());
    encoder->encode(text + '\n', buffer.data);

    //  Issue a "svc"
    WriteToOperatorSystemCall systemCall(this, &buffer);
    return makeSystemCall(&systemCall);
}

//////////
//  Operations
void EmulatedProcess::markSystemCallComplete()
{
    _systemCallCompletionSemaphore.release();
}

//////////
//  EmulatedProcess::_WorkerThread
EmulatedProcess::_WorkerThread::_WorkerThread(EmulatedProcess * emulatedProcess)
    :   _emulatedProcess(emulatedProcess)
{
    Q_ASSERT(_emulatedProcess != nullptr);
}

EmulatedProcess::_WorkerThread::~_WorkerThread()
{
}

void EmulatedProcess::_WorkerThread::run()
{
    _emulatedProcess->_state = Process::State::Active;
    try
    {
        _emulatedProcess->_exitCode = _emulatedProcess->run();
        _emulatedProcess->_state = Process::State::Finished;
    }
    catch (...)
    {
        _emulatedProcess->_exitCode = 4;    //  TODO properly!
        _emulatedProcess->_state = Process::State::Finished;
    }
}

//  End of emuone-scp360/EmulatedProcess.cpp
