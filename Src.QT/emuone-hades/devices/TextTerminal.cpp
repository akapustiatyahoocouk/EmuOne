//
//  emuone-hades/devices/TextTerminal.cpp - emuone::hades::devices::TextTerminal class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-hades/API.hpp"
using namespace emuone::hades::devices;

//////////
//  Construction/destruction
TextTerminal::TextTerminal()
{
}

TextTerminal::~TextTerminal()
{
    if (_notificationThread != nullptr)
    {   //  Make sure it's not running
        _notificationThread->terminate();
        delete _notificationThread;
    }
}

//////////
//  emuone::core::IComponent
auto TextTerminal::type() const -> emuone::core::IComponentType *
{
    return Type::instance();
}

QString TextTerminal::displayName() const
{
    return "Text terminal #" +
           emuone::util::toString(_terminalNumber) +
           " (" +
           emuone::util::toString(_columns) +
           "x" +
           emuone::util::toString(_rows) +
           ")";
}

auto TextTerminal::createEditor(QWidget * parent) -> emuone::core::ComponentEditor *
{
    return new TextTerminalEditor(parent, this);
}

void TextTerminal::saveConfiguration(QDomElement & element) const
{
    IDevice::saveConfiguration(element);

    element.setAttribute("TerminalNumber", _terminalNumber);
    element.setAttribute("Columns", _columns);
    element.setAttribute("Rows", _rows);
}

void TextTerminal::restoreConfiguration(const QDomElement & element)
{
    IDevice::restoreConfiguration(element);

    _terminalNumber =
        emuone::util::fromString<int>(
            element.attribute("TerminalNumber"),
        _terminalNumber);
    _columns =
        emuone::util::fromString<int>(
            element.attribute("Columns"),
            _columns);
    _rows =
        emuone::util::fromString<int>(
            element.attribute("Rows"),
            _rows);
}

//////////
//  emuone::core::IComponent (state control)
TextTerminal::State TextTerminal::state() const
{
    emuone::util::Lock _(stateGuard);
    return _state;
}

void TextTerminal::connect()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Constructed || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  There is nothing to connect to
    //  Perform state change
    _state = State::Connected;
}

void TextTerminal::disconnect() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  Nothing to do
        return;
    }
    //  There is nothing to disconnect
    //  Perform state change
    _state = State::Constructed;
}

void TextTerminal::initialize()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Connected || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  There is nothing to intiialize
    //  Perform state change
    _state = State::Initialized;
}

void TextTerminal::deinitialize() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  Nothing to do
        return;
    }
    //  There is nothing to deinitialize
    //  Perform state change
    _state = State::Connected;
}

void TextTerminal::start()
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Initialized || !isBound())
    {   //  OOPS!
        Q_ASSERT(false);    //  TODO throw
    }
    //  Start notification thread
    Q_ASSERT(_notificationThread == nullptr);
    _notificationThread = new _NotificationThread(this);
    _notificationThread->start();
    //  Perform state change
    _state = State::Running;
}

void TextTerminal::stop() noexcept
{
    emuone::util::Lock _(stateGuard);

    //  Check preconditions
    if (_state != State::Running || !isBound())
    {   //  Nothing to do
        return;
    }
    //  Stop the notification thread, cleanly if possible...
    Q_ASSERT(_notificationThread != nullptr);
    _notificationThread->requestStop();
    _notificationThread->wait(_NotificationThread::WaitChunkMs * 5);
    if (_notificationThread->isRunning())
    {   //  ...or dirtyli if not
        _notificationThread->terminate();
        _notificationThread->wait(_NotificationThread::WaitChunkMs * 5);
    }
    delete _notificationThread;
    _notificationThread = nullptr;
    //  Perform state change
    _state = State::Initialized;
}

//////////
//  emuone::core::IDevice
auto TextTerminal::sendCommand(
    IDevice::Command * command
    ) -> SendCommandOutcome
{
    if (state() != State::Running)
    {   //  OOPS
        return SendCommandOutcome::InvalidDeviceState;
    }

    //  We use double-dispatch for speed, but make sure
    //  at least the Debug build performs the validity checks
    Q_ASSERT(dynamic_cast<Command*>(command) != nullptr);
    //  TODO Command * cmd = static_cast<Command*>(command);
    //  TODO implement properly
    return SendCommandOutcome::UnsupportedCommand;
}

//////////
//  Operations (configuration)
bool TextTerminal::isValidTerminalNumber(int terminalNumber)
{
    return terminalNumber >= MinTerminalNumber && terminalNumber <= MaxTerminalNumber;
}

bool TextTerminal::isValidColumns(int columns)
{
    return columns >= MinColumns && columns <= MaxColumns;
}

bool TextTerminal::isValidRows(int rows)
{
    return rows >= MinRows && rows <= MaxRows;
}

int TextTerminal::terminalNumber() const
{
    emuone::util::Lock _(stateGuard);
    return _terminalNumber;
}

void TextTerminal::setTerminalNumber(int terminalNumber)
{
    emuone::util::Lock _(stateGuard);
    Q_ASSERT(isValidTerminalNumber(terminalNumber));    //  TODO throw instead

    _terminalNumber = terminalNumber;
}

int TextTerminal::columns() const
{
    return _columns;
}

void TextTerminal::setColumns(int columns)
{
    emuone::util::Lock _(stateGuard);
    Q_ASSERT(isValidColumns(columns));  //  TODO throw instead

    _columns = columns;
}

int TextTerminal::rows() const
{
    return _rows;
}

void TextTerminal::setRows(int rows)
{
    emuone::util::Lock _(stateGuard);
    Q_ASSERT(isValidRows(rows));    //  TODO throw instead

    _rows = rows;
}

//////////
//  TextTerminal::Type
EMUONE_IMPLEMENT_SINGLETON(TextTerminal::Type)
TextTerminal::Type::Type() {}
TextTerminal::Type::~Type() {}

QString TextTerminal::Type::mnemonic() const
{
    return "emuone::hades::devices::TextTerminal";
}

QString TextTerminal::Type::displayName() const
{
    return "Text terminal";
}

auto TextTerminal::Type::category() const -> emuone::core::IComponentCategory *
{
    return emuone::core::StandardComponentCategories::UiDevices::instance();
}

bool TextTerminal::Type::isCompatibleWith(emuone::core::IArchitecture * architecture) const
{
    return architecture == Architecture::instance();
}

bool TextTerminal::Type::isCompatibleWith(emuone::core::IVirtualMachineType * virtualMachineType) const
{
    return virtualMachineType == emuone::core::StandardVirtualMachineTypes::VirtualMachine::instance();
}

bool TextTerminal::Type::isPersistable() const
{
    return false;
}

auto TextTerminal::Type::createComponent() -> TextTerminal *
{
    return new TextTerminal();
}

//////////
//  TextTerminal::_NotificationThread
void TextTerminal::_NotificationThread::run()
{
    while (!_stopRequested)
    {
        Response * response;
        if (_pendingResponses.tryDequeue(response, WaitChunkMs))
        {   //  Dispatch one response...
            _extfs->dispatchResponse(response); //  TODO may throw
            delete response;
            //  ...and all subsequent ones which are ready
            while (_pendingResponses.tryDequeue(response, 0))
            {
                _extfs->dispatchResponse(response); //  TODO may throw
                delete response;
            }
        }
    }
}

//  End of emuone-hades/devices/TextTerminal.cpp
