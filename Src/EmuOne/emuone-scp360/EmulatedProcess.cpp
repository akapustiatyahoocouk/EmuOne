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
ErrorCode EmulatedProcess::writeToOperator(const QString & text)
{
    if (scp()->_operatorsConsole == nullptr)
    {   //  OOPS! No operator console
        return ErrorCode::ERR_SUP;
    }

    //  The "operator console" is a text device that expects EBCDIC bytes.
    std::unique_ptr<util::CharacterSet::Encoder> encoder(util::Cp037CharacterSet::getInstance()->createEncoder());
    QByteArray ebcdic;
    encoder->encode(text + '\n', ebcdic);

    //  Instruct device driver to begin a "write".
    DeviceDriver * deviceDriver = scp()->_deviceDrivers[scp()->_operatorsConsole];
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
