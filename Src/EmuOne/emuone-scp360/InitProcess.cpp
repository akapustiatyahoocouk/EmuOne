//
//  emuone-scp360/InitProcess.cpp
//
//  The scp360::InitProcess class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
InitProcess::InitProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
    :   EmulatedProcess(scp, id, name, flags, parent)
{
}

InitProcess::~InitProcess()
{
}

//////////
//  EmulatedProcess
EmulatedApplication * InitProcess::application() const
{
    return InitProcess::InitApplication::getInstance();
}

uint16_t InitProcess::run()
{
    return 0;
}

//////////
//  InitProcess::InitApplication
IMPLEMENT_SINGLETON(InitProcess::InitApplication)
InitProcess::InitApplication::InitApplication() {}
InitProcess::InitApplication::~InitApplication() {}

QString InitProcess::InitApplication::mnemonic() const
{
    return "INIT";
}

QString InitProcess::InitApplication::displayName() const
{
    return "INIT";
}


//////////
//  EmulatedApplication
EmulatedProcess * InitProcess::InitApplication::createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
{
    return new InitProcess(scp, id, name, flags, parent);
}

//  End of emuone-scp360/InitProcess.cpp
