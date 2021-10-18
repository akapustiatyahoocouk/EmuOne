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
    :   Process(scp, id, name, flags, parent),
        systemCalls(this)
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

void EmulatedProcess::stop(ErrorCode exitCode)
{
    if (_state != Process::State::Prepared &&
        _state != Process::State::Finished)
    {   //  The process is supposed to have started but not yet finished
        if (_workerThread != nullptr && _workerThread->isRunning())
        {
            if (scp()->_systemCallsInProgress.contains(this))
            {   //  The "emulatedProcess" is waiting for a system call to complete
                SystemCall * systemCall = scp()->_systemCallsInProgress[this];
                systemCall->setOutcome(ErrorCode::ERR_UNK);
                _workerThread->requestInterruption();
                _systemCallCompletionSemaphore.release();   //  in case process is in a system call
                _workerThread->wait();
            }
            else
            {   //  The "emulatedProcess" is running
                _workerThread->requestInterruption();
                _workerThread->wait();
            }
        }
        this->_exitCode = exitCode;
    }
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
    catch (ErrorCode errorCode)
    {   //  Process was force-terminated
        _emulatedProcess->_exitCode = errorCode;
        _emulatedProcess->_state = Process::State::Finished;
    }
    catch (...)
    {
        _emulatedProcess->_exitCode = ErrorCode::ERR_UNK;
        _emulatedProcess->_state = Process::State::Finished;
    }
}

//  End of emuone-scp360/EmulatedProcess.cpp
