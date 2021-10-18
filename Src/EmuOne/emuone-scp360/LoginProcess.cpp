//
//  emuone-scp360/LoginProcess.cpp
//
//  The scp360::LoginProcess class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
LoginProcess::LoginProcess(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
    :   EmulatedProcess(scp, id, name, flags, parent)
{
}

LoginProcess::~LoginProcess()
{
}

//////////
//  EmulatedProcess
EmulatedApplication * LoginProcess::application() const
{
    return LoginProcess::Application::getInstance();
}

ErrorCode LoginProcess::run()
{
    return ErrorCode::ERR_OK;
}

//////////
//  LoginProcess::Application
IMPLEMENT_SINGLETON(LoginProcess::Application)
LoginProcess::Application::Application() {}
LoginProcess::Application::~Application() {}

QString LoginProcess::Application::mnemonic() const
{
    return "LOGIN";
}

QString LoginProcess::Application::displayName() const
{
    return "LOGIN";
}


//////////
//  EmulatedApplication
EmulatedProcess * LoginProcess::Application::createInstance(Scp * scp, uint16_t id, const QString & name, Process::Flags flags, Process * parent)
{
    return new LoginProcess(scp, id, name, flags, parent);
}

//  End of emuone-scp360/LoginProcess.cpp
