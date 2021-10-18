//
//  emuone-360/Ibm2741.cpp
//
//  ibm360::Ibm2741 class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm2741::Ibm2741(const QString & name, uint16_t address)
    :   Device(name, address)
{
}

Ibm2741::~Ibm2741()
{
    //  Destroy live Editors
    for (Ibm2741Editor * editor : _editors)
    {
        delete editor;
    }
}

//////////
//  Component
core::ComponentType * Ibm2741::type() const
{
    return Ibm2741::Type::getInstance();
}

ComponentEditor * Ibm2741::createEditor(QWidget * parent)
{
    Ibm2741Editor * editor = new Ibm2741Editor(this, parent);;
    _editors.append(editor);
    return editor;
}

QString Ibm2741::shortStatus() const
{
    return "@" + ("000" + QString::number(address(), 16)).right(3).toUpper();
}

core::ComponentUi * Ibm2741::createUi()
{
    return new Ibm2741::Ui(this);
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Ibm2741::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Ibm2741::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Ibm2741::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _deviceState = DeviceState::Idle;

    //  Done
    _state = State::Initialised;
}

void Ibm2741::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _workerThread = new _WorkerThread(this);
    _workerThread->start();

    //  Done
    _state = State::Running;
}

void Ibm2741::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _workerThread->requestStop();   //  this stops it eventually
    _workerThread = nullptr;

    //  Done
    _state = State::Initialised;
}

void Ibm2741::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    _deviceState = DeviceState::NotOperational;

    //  Done
    _state = State::Connected;
}

void Ibm2741::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void Ibm2741::serialiseConfiguration(QDomElement & configurationElement) const
{
    Device::serialiseConfiguration(configurationElement);
}

void Ibm2741::deserialiseConfiguration(QDomElement & configurationElement)
{
    Device::deserialiseConfiguration(configurationElement);
}

//////////
//  Operations
Ibm2741::ErrorCode Ibm2741::beginReset(IoCompletionListener * completionListener)
{
    Q_ASSERT(completionListener != nullptr);

    _requestQueue.enqueue(new _ResetRequest(completionListener));
    return Ibm2741::ErrorCode::Success;
}

Ibm2741::ErrorCode Ibm2741::beginWrite(const util::Buffer * buffer, TransferCompletionListener * completionListener)
{
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(completionListener != nullptr);

    //  Can we ?
    if (_deviceState != DeviceState::Idle)
    {   //  No!
        return ErrorCode::Busy;
    }
    //  Send a "write" request to the worker thread
    _requestQueue.enqueue(new _WriteRequest(buffer, completionListener));
    return Ibm2741::ErrorCode::Success;
}

Ibm2741::ErrorCode Ibm2741::beginWriteBlock(const util::Buffer * buffer, TransferCompletionListener * completionListener)
{
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(completionListener != nullptr);

    //  Can we ?
    if (_deviceState != DeviceState::Idle)
    {   //  No!
        return ErrorCode::Busy;
    }
    //  Send a "write" request to the worker thread
    _requestQueue.enqueue(new _WriteBlockRequest(buffer, completionListener));
    return Ibm2741::ErrorCode::Success;
}

//////////
//  Ibm2741::Type
IMPLEMENT_SINGLETON(Ibm2741::Type)
Ibm2741::Type::Type() {}
Ibm2741::Type::~Type() {}

QString Ibm2741::Type::mnemonic() const
{
    return "ibm360::Ibm2741";
}

QString Ibm2741::Type::displayName() const
{
    return "IBM 2741 terminal";
}

core::ComponentCategory * Ibm2741::Type::category() const
{
    return core::StandardComponentCategories::Terminals::getInstance();
}

bool Ibm2741::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Ibm2741 * Ibm2741::Type::createComponent()
{
    return new Ibm2741("IBM 2741", 0x000);
}

//////////
//  Ibm2741::Ui
Ibm2741::Ui::Ui(Ibm2741 * ibm2741)
    :   core::ComponentUi(ibm2741),
        _fullScreenWidget(new Ibm2741FullScreenWidget(ibm2741))
{
}

Ibm2741::Ui::~Ui()
{
}

core::FullScreenWidgetList Ibm2741::Ui::fullScreenWidgets()
{
    core::FullScreenWidgetList result;
    result.append(_fullScreenWidget);
    return result;
}

//////////
//  Ibm2741::_WorkerThread
Ibm2741::_WorkerThread::_WorkerThread(Ibm2741 * ibm2741)
    :   _ibm2741(ibm2741) ,
        _ebcdicDecoder(util::Cp037CharacterSet::getInstance()->createDecoder())
{
}

Ibm2741::_WorkerThread::~_WorkerThread()
{
    delete _ebcdicDecoder;
}

void Ibm2741::_WorkerThread::run()
{
    _ibm2741->_deviceState = DeviceState::Idle;
    while (!_stopRequested)
    {
        _Request * request;
        if (!_ibm2741->_requestQueue.tryDequeue(request, 100))
        {
            continue;
        }
        if (_ResetRequest * resetRequest = dynamic_cast<_ResetRequest*>(request))
        {
            _handleResetRequest(resetRequest);
        }
        else if (_WriteRequest * writeRequest = dynamic_cast<_WriteRequest*>(request))
        {
            _handleWriteRequest(writeRequest);
        }
        else if (_WriteBlockRequest * writeBlockRequest = dynamic_cast<_WriteBlockRequest*>(request))
        {
            _handleWriteBlockRequest(writeBlockRequest);
        }
        else
        {   //  OOPS! Invalid request!
            Q_ASSERT(false);
        }
    }
}

void Ibm2741::_WorkerThread::_handleResetRequest(const _ResetRequest * request)
{
    _ibm2741->_deviceState = DeviceState::Resetting;
    QThread::sleep(5);
    _ibm2741->_deviceState = DeviceState::Idle;
    request->completionListener->ioCompleted(ErrorCode::Success);
}

void Ibm2741::_WorkerThread::_handleWriteRequest(const _WriteRequest * request)
{
    _ibm2741->_deviceState = DeviceState::Writing;
    QChar ch;
    for (int i = 0; i < request->buffer->size(); i++)
    {   //  Translate EBCDIC to ASCII...
        _decodeBuffer.clear();
        _decodeBuffer.append(request->buffer->at(i));
        if (_ebcdicDecoder->decode(_decodeBuffer, 0, ch) == 0)
        {
            ch = '?';
        }
        //  ...print...
        if (ch == '\n')
        {   //  Start a new line
            _ibm2741->_contents.append("");
        }
        else
        {
            if (_ibm2741->_contents.isEmpty())
            {
                _ibm2741->_contents.append(QString(1, ch));
            }
            else if (_ibm2741->_contents[_ibm2741->_contents.size() - 1].length() >= 79)
            {   //  Printing the 80th column - need to start a new line
                _ibm2741->_contents[_ibm2741->_contents.size() - 1] += ch;
                _ibm2741->_contents.append("");
            }
            else
            {   //  Adding to the current line
                _ibm2741->_contents[_ibm2741->_contents.size() - 1] += ch;
            }
        }
        //  ...and simulate delay
        QThread::msleep(70);    //  14.1 CPS

    }
    _ibm2741->_deviceState = DeviceState::Idle;
    request->completionListener->transferCompleted(request->buffer->size(), ErrorCode::Success);
}

void Ibm2741::_WorkerThread::_handleWriteBlockRequest(const _WriteBlockRequest * request)
{
    _ibm2741->_deviceState = DeviceState::Writing;
    QChar ch;
    for (int i = 0; i < request->buffer->size(); i++)
    {   //  Translate EBCDIC to ASCII...
        _decodeBuffer.clear();
        _decodeBuffer.append(request->buffer->at(i));
        if (_ebcdicDecoder->decode(_decodeBuffer, 0, ch) == 0)
        {
            ch = '?';
        }
        //  ...print...
        if (ch == '\n')
        {   //  Start a new line
            _ibm2741->_contents.append("");
        }
        else
        {
            if (_ibm2741->_contents.isEmpty())
            {
                _ibm2741->_contents.append(QString(1, ch));
            }
            else if (_ibm2741->_contents[_ibm2741->_contents.size() - 1].length() >= 79)
            {   //  Printing the 80th column - need to start a new line
                _ibm2741->_contents[_ibm2741->_contents.size() - 1] += ch;
                _ibm2741->_contents.append("");
            }
            else
            {   //  Adding to the current line
                _ibm2741->_contents[_ibm2741->_contents.size() - 1] += ch;
            }
        }
        //  ...and simulate delay
        QThread::msleep(70);    //  14.1 CPS

    }
    _ibm2741->_contents.append(""); //  ...to start a new line
    _ibm2741->_deviceState = DeviceState::Idle;
    request->completionListener->transferCompleted(request->buffer->size(), ErrorCode::Success);
}

//  End of emuone-360/Ibm2741.cpp
