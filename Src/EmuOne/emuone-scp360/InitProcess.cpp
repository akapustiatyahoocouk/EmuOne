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
InitProcess::InitProcess(Scp * scp, uint16_t id)
    :   EmulatedProcess(scp,
                        id,
                        InitProcess::InitApplication::getInstance()->mnemonic(),
                        Flags::System,
                        nullptr,
                        InitProcess::InitApplication::getInstance())
{
}

InitProcess::~InitProcess()
{
}

//////////
//  EmulatedProcess
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

//  End of emuone-scp360/InitProcess.cpp
